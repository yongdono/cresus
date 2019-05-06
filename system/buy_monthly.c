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

#include "engine/engine.h"
#include "engine/common_opt.h"
#include "framework/verbose.h"
#include "framework/timeline.h"
#include "input/input_wrapper.h"

static int amount = 250;
static int occurrence = 1;

static int feed(struct engine *e,
		struct timeline *t,
		struct timeline_track_entry *entry)
{
  /* Step by step loop */
  static int last_month = -1;
  
  /* Execute */
  int month = TIME_GET_MONTH(entry->slice->time);
  if(month != last_month && !(month % occurrence))
    engine_set_order(e, BUY, amount, CASH, NULL);
  
  last_month = month;
  return 0;
}

static int timeline_create(struct timeline *t,
                           const char *filename,
                           const char *type)
{
  /*
   * Data
   */
  struct input *input;
  if((input = input_wrapper_create(filename, type))){
    /* Create tracks */
    struct timeline_track *track0;
    timeline_track_alloc(track0, 0);
    /* Create indicators */
    /* ... */
    /* Add to timeline */
    timeline_init(t);
    timeline_add_track(t, track0, input);
    return 0;
  }
  
  return -1;
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
  
  struct engine engine;
  struct timeline timeline;

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
  
  if((timeline_create(&timeline, filename, opt.input_type.s))){
    engine_init(&engine, &timeline);
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
