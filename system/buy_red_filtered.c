/*
 * Cresus EVO - buy_red_filtered.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 07/04/2016
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
#include "framework/common_opt.h"

typedef enum {
  STATE_NORMAL,
  STATE_PRIME
} state_t;

/* n consecutive red candles */
static int level_min = 1;
/* amount to buy */
static double amount = 500.0;
/* Boundaries */
static time_info_t time_min = VAL_YEAR(1900);
static time_info_t time_max = TIME_MAX;
/* State machine */
static state_t state = STATE_NORMAL;

static int feed(struct engine *e,
		struct timeline *t,
		struct timeline_entry *entry)
{
  /* Step by step loop */
  static int level = 0;
  struct candle *c = __timeline_entry_self__(entry);
  
  /* Execute */
  if(candle_is_red(c)) level++;
  else level = 0;
  
  if(level >= level_min)
    state = STATE_PRIME;
  
  if(state == STATE_PRIME && !level){
    /* Trigger buy order */
    engine_set_order(e, BUY, amount, CASH, NULL);
    state = STATE_NORMAL;
  }
  
  return 0;
}

static struct timeline *timeline_create(const char *filename,
                                        const char *type)
{
  /*
   * Data
   */
  struct inwrap *inwrap;
  struct timeline *timeline;
  inwrap_t t = inwrap_t_from_str(type);
  
  if(inwrap_alloc(inwrap, filename, t, TIME_MIN, time_max)){
    if(timeline_alloc(timeline, "buy_red_filtered", __input__(inwrap))){
      /* Ok */
      return timeline;
    }
  }
  
  return NULL;
}

static void print_usage(const char *argv0)
{
  fprintf(stderr, "Usage: %s -o type filename\n", argv0);
}

int main(int argc, char **argv)
{
  VERBOSE_LEVEL(INFO);
  
  /*
   * Data
   */
  int c;
  char *filename, *type;
  struct common_opt opt;
  
  struct timeline *t;
  struct engine engine;

  /* Check arguments */
  if(argc < 2)
    goto usage;
  
  /* Options */
  common_opt_init(&opt);
  if(common_opt_getopt(&opt, argc, argv) < 0)
    goto usage;
  
  if(opt.start_time.set) time_min = opt.start_time.value.t;
  if(opt.end_time.set) time_max = opt.end_time.value.t;
  if(opt.fixed_amount.set) amount = opt.fixed_amount.value.i;
  if(!opt.input_type.set) goto usage;

  /* Specific options */
  while((c = getopt(argc, argv, "l:")) != -1){
    switch(c){
    case 'l': level_min = atoi(optarg); break;
    default: break; /* Ignore */
    }
  }
  
  /* Filename is only real param */
  filename = argv[optind];
  
  if((t = timeline_create(filename, opt.input_type.value.s))){
    engine_init(&engine, t);
    /* Opt */
    engine_set_filter(&engine, time_min);
    engine_set_transaction_fee(&engine, 2.50);
    /* Run */
    engine_run(&engine, feed);

    /* Print some info */
    engine_display_stats(&engine);

    /* Are there pending orders ? (FIXME : dedicated function in engine ?) */
    struct position *p;
    __list_for_each__(&engine.list_position, p)
      if(p->status == POSITION_REQUESTED)
	PR_ERR("Buy now ! Quick ! Schnell !\n");
    
    /* TODO : Don't forget to release everything */
    engine_release(&engine);
  }
  
  return 0;

 usage:
  print_usage(argv[0]);
  return -1;
}
