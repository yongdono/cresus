/*
 * Cresus EVO - basics.c
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

static int level_min = 1;
static time_info_t year_min = VAL_YEAR(1900);

static int feed(struct engine *e,
		struct timeline *t,
		struct timeline_entry *entry)
{
  /* Step by step loop */
  static int n = 0, level = 0;
  struct candle *c = __timeline_entry_self__(entry);
  
  /* Execute */
  if(candle_is_red(c)) level++;
  else level = 0;
  
  if(level >= level_min)
    engine_place_order(e, ORDER_BUY, ORDER_BY_AMOUNT, 500);
  
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
    if(timeline_alloc(timeline, "buy_red_filtered", __input__(inwrap))){
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

  while((c = getopt(argc, argv, "o:n:l:")) != -1){
    switch(c){
    case 'o': type = optarg; break;
    case 'n': year_min = VAL_YEAR(atoi(optarg)); break;
    case 'l': level_min = atoi(optarg); break;
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
    
    /* TODO : Don't forget to release everything */
    engine_release(&engine);
  }
  
  return 0;
}
