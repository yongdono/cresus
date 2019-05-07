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

#include "engine/engine_v2.h"
#include "engine/common_opt.h"

#include "framework/verbose.h"
#include "framework/timeline.h"
#include "input/input_wrapper.h"

#include "indicator/lowest.h"

static int amount = 250;
static int occurrence = 1;

/* UIDs */
#define UID_TRACK0        0
#define UID_TRACK0_LOWEST 1

/* For each indicator */
static void feed2(struct engine_v2 *engine,
                  struct timeline_track_n3 *track_n3,
                  struct indicator_n3 *indicator_n3)
{
  struct lowest_n3 *lowest_n3 = (void*)indicator_n3;
  unique_id_t uid = __slist_by_uid__(indicator_n3)->uid;
  
  switch(uid){
  case UID_TRACK0_LOWEST:
    PR_INFO("feed2: %s (lowest %.2lf)\n",
            timeline_track_n3_str(track_n3),
            lowest_n3->value);
    break;
    
  default:
    PR_ERR("feed2: unknown indicator uid %d\n", uid);
    break;
  }
}

/* For each track */
static void feed1(struct engine_v2 *engine,
                  struct timeline_slice *slice,
                  struct timeline_track_n3 *track_n3)
{
  /* Step by step loop */
  static int last_month = -1;
  
  struct indicator_n3 *indicator_n3;
  timeline_track_n3_for_each_indicator_n3(track_n3, indicator_n3)
    feed2(engine, track_n3, indicator_n3);

  /* For each track */
  int month = TIME64_GET_MONTH(slice->time);
  if(month != last_month && !(month % occurrence)){
    /* We can't get lowest here :( */
    PR_WARN("%s - BUY %d\n", timeline_track_n3_str(track_n3), amount);
  }
  
  last_month = month;
}

/* For each slice */
static int feed(struct engine_v2 *engine,
		struct timeline_slice *slice)
{
  /* Execute by stages */
  struct timeline_track_n3 *track_n3;
  timeline_slice_for_each_track_n3(slice, track_n3)
    feed1(engine, slice, track_n3);
  
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
    struct lowest *lowest;
    lowest_alloc(lowest, UID_TRACK0_LOWEST, 50);
    timeline_track_add_indicator(track0, __indicator__(lowest));
    /* Add to timeline */
    timeline_init(t);
    timeline_add_track(t, track0, input);
    /* Execute timeline */
    timeline_run_and_sync(t);
    return 0;
  }
  
  return -1;
}

int main(int argc, char **argv)
{
  VERBOSE_LEVEL(DBG);
  
  /*
   * Data
   */
  int c;
  char *filename;
  
  struct common_opt opt;
  struct engine_v2 engine;
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
  
  if(!(timeline_create(&timeline, filename, opt.input_type.s))){
    engine_v2_init(&engine, &timeline);
    engine_v2_set_common_opt(&engine, &opt);
    /* Run */
    engine_v2_run(&engine, feed);
    
    /* Print some info */
    //engine_display_stats(&engine);
    
    /* TODO : Don't forget to release everything */
    engine_v2_release(&engine);
  }
  
  return 0;

 usage:
  fprintf(stdout, "Usage: %s -o type filename\n", argv[0]);
  return -1;
}
