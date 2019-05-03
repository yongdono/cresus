/*
 * Cresus EVO - buy_monthly.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 25/02/2018
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
#include "engine/common_opt.h"
#include "framework/verbose.h"

static int amount = 250;
static int occurrence = 1;

static int feed(struct engine *e,
		struct timeline *t,
		struct timeline_entry *entry)
{
  /* Step by step loop */
  static int last_month = -1;
  struct candle *c = (void*)entry;
  
  /* Execute */
  int month = TIME_GET_MONTH(entry->time);
  if(month != last_month && !(month % occurrence))
    engine_set_order(e, BUY, amount, CASH, NULL);
  
  last_month = month;
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
  
  if(inwrap_alloc(inwrap, filename, t)){
    if(timeline_alloc(timeline, "buy_monthly")){
      /* Ok */
      timeline_load(timeline, __input__(inwrap));
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
  char *filename;
  struct common_opt opt;
  
  struct timeline *t;
  struct engine engine;

  if(argc < 2)
    goto usage;

  /* Options */
  common_opt_init(&opt, "m:");
  while((c = common_opt_getopt(&opt, argc, argv)) != -1){
    switch(c){
    case 'm': occurrence = atoi(optarg); break;
    default: break;
    }
  }
  
  /* Command line params */
  filename = argv[optind];
  if(opt.fixed_amount.set) amount = opt.fixed_amount.i;
  if(!opt.input_type.set) goto usage;
  
  if((t = timeline_create(filename, opt.input_type.s))){
    engine_init(&engine, t);
    engine_set_common_opt(&engine, &opt);
    /* Run */
    engine_run(&engine, feed);
    
    /* Print some info */
    engine_display_stats(&engine);
    
    /* TODO : Don't forget to release everything */
    engine_release(&engine);
  }
  
  return 0;

 usage:
  fprintf(stdout, "Usage: %s -o type filename\n", argv[0]);
  return -1;
}
