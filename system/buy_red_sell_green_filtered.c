/*
 * Cresus EVO - buy_red_sell_green.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 18/04/2018
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 *
 * Development sample code
 * Used to determine if interfaces are coherent for the "user"
 * And maybe to implement some kind of script commands 
 *
 */

#include <stdio.h>
#include <getopt.h>
#include <math.h>

#include "input/inwrap.h"
#include "engine/engine.h"
#include "framework/verbose.h"

typedef enum {
  STATE_NORMAL,
  STATE_PRIME_BUY,
  STATE_PRIME_SELL
} state_t;

static int level_buy_min = 1;
static int level_sell_min = 1;
static time_info_t year_min = VAL_YEAR(1900);
static state_t state = STATE_NORMAL;

static int feed(struct engine *e,
		struct timeline *t,
		struct timeline_entry *entry)
{
  /* Step by step loop */
  static int level_buy = 0, level_sell = 0;
  struct candle *c = __timeline_entry_self__(entry);
  
  /* Execute */
  if(candle_is_red(c)){
    level_buy++;
    level_sell = 0;
  }else{
    level_sell++;
    level_buy = 0;
  }

  /* State machine */
  if(level_buy >= level_buy_min) state = STATE_PRIME_BUY;
  if(level_sell >= level_sell_min) state = STATE_PRIME_SELL;
  
  /* Trigger buy order */
  if(state == STATE_PRIME_BUY && !level_buy){
    engine_set_order(e, BUY, 500, CASH, NULL);
    state = STATE_NORMAL;
  }
  
  /* Trigger sell order */
  if(state == STATE_PRIME_SELL && !level_sell){
    engine_set_order(e, SELL, 500, CASH, NULL);
    state = STATE_NORMAL;
  }
  
  return 0;
}

static struct timeline *timeline_create(const char *filename, const char *type)
{
  /*
   * Data
   */
  struct inwrap *inwrap;
  struct timeline *timeline;
  inwrap_t t = inwrap_t_from_str(type);
  
  if(inwrap_alloc(inwrap, filename, t, TIME_MIN, TIME_MAX)){
    if(timeline_alloc(timeline, "buy_red_sell_green_filtered",
                      __input__(inwrap))){
      /* Ok */
      return timeline;
    }
  }
  
  return NULL;
}

int main(int argc, char **argv)
{
  VERBOSE_LEVEL(INFO);
  
  /*
   * Data
   */
  int c;
  char *filename, *type;
  
  struct timeline *t;
  struct engine engine;

  if(argc < 2){
    fprintf(stdout, "Usage: %s -o type filename\n", argv[0]);
    return -1;
  }

  while((c = getopt(argc, argv, "o:n:b:s:")) != -1){
    switch(c){
    case 'o': type = optarg; break;
    case 'n': year_min = VAL_YEAR(atoi(optarg)); break;
    case 'b': level_buy_min = atoi(optarg); break;
    case 's': level_sell_min = atoi(optarg); break;
    default:
      PR_ERR("Unknown option %c\n", c);
      return -1;
    }
  }

  /* Filename is only real param */
  filename = argv[optind];
  
  if((t = timeline_create(filename, type))){
    engine_init(&engine, t);
    /* Opt */
    engine_set_filter(&engine, year_min);
    engine_set_transaction_fee(&engine, 2.50);
    /* Run */
    engine_run(&engine, feed);

    /* Print some info */
    engine_display_stats(&engine);

    /* Are there pending orders ? (FIXME : dedicated function in engine ?) */
    struct position *p;
    __list_for_each__(&engine.list_position, p){
      if(p->status == POSITION_REQUESTED){
	switch(p->type){
	case BUY: PR_ERR("Buy now ! Quick ! Schnell !\n");
	case SELL: PR_ERR("Sell now ! Quick ! Schnell !\n");
	default: PR_ERR("C'mon do something\n");
	}
      }
    }
    
    /* TODO : Don't forget to release everything */
    engine_release(&engine);
  }
  
  return 0;
}
