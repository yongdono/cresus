/*
 * Cresus EVO - cluster.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 22/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "cluster.h"
#include "candle.h"

int cluster_init(struct cluster *c, const char *name,
		 time_info_t time_min, time_info_t time_max) {

  /* Super */
  __timeline_super__(c, name, NULL);
  __slist_head_init__(&c->slist_timeline);
  /* Set options */
  c->ref = &__timeline__(c)->list_entry;
  c->time = time_min;
  c->time_max = time_max;
   /* FIXME : make other granularities available */
  c->g = GRANULARITY_DAY;    

  return 0;
}

void cluster_release(struct cluster *c) {

  __timeline_release__(c);
  __slist_head_release__(&c->slist_timeline);
  c->ref = NULL;
}

int cluster_add_timeline(struct cluster *c, struct timeline *t) {

  __slist_insert__(&c->slist_timeline, t);
  return 0;
}

static int cluster_create_index(struct cluster *c, struct candle *candle) {
  
  struct candle *current;
  struct timeline_entry *entry;
  
  if(!list_is_head(&__timeline__(c)->list_entry, c->ref) &&
     (entry = timeline_entry_find(__list_self__(c->ref),
				  __timeline_entry__(candle)->time))){
    /* Just continue filling candle */
    current = __timeline_entry_self__(entry);
    current->open += candle->open;
    current->close += candle->close;
    current->high += candle->high;
    current->low += candle->low;
    current->volume += candle->volume;
    
  }else{
    /* Create missing entry */
    if(candle_alloc(current, __timeline_entry__(candle)->time,
		    __timeline_entry__(candle)->granularity,
		    candle->open, candle->close, candle->high,
		    candle->low, candle->volume)){
      /* Add it to list */
      list_add_tail(&__timeline__(c)->list_entry,
		    __list__(__timeline_entry__(current)));
      /* Update ref for next round */
      c->ref = __list__(__timeline_entry__(current));
      
    }else
      return -1;
  }
  
  return 0;
}

static int cluster_prepare_step(struct cluster *c, time_info_t time) {

  struct timeline *t;
  struct candle *candle;

  if(!candle_alloc(candle, time, c->g, 0, 0, 0, 0, 0))
    return -1;
  
  __slist_for_each__(&c->slist_timeline, t){
    struct timeline_entry *entry;
    /* Why not use granularity here to merge candles in timeline object ? */
    if((entry = timeline_entry_by_time(t, time))){
      /* Merge candles */
      /* TODO : What if there's no more data ? */
      struct candle *c2 = __timeline_entry_self__(entry);
      candle_merge(candle, c2);
      
    }else{
      /* If we can't get all the data for 1 slot, we let it down and
       * check for a "complete" one */
      candle_free(candle);
      fprintf(stderr, "No data available for %x\n", time);
      return 0;
    }
  }
  /* Add data to list */
  __list_add_tail__(&__timeline__(c)->list_entry,
		    __timeline_entry__(candle));

  return 1;
}

static int cluster_execute_step(struct cluster *c, time_info_t time) {

  struct timeline *t;
  /* Execute indicators */
  __slist_for_each__(&c->slist_timeline, t){
    /* Step all timelines */
    struct timeline_entry *entry;
    if((entry = timeline_step(t))){
      struct indicator_entry *indicator;
      struct candle *candle = __timeline_entry_self__(entry);
      /* Indicators management */
      __slist_for_each__(&candle->slist_indicator, indicator){
	/* TODO : Use function pointer ?
	 * Find a way to exploit all this data
	 */
      }
    }
  }

  return 0;
}

int cluster_step(struct cluster *c) {

  int ret;
  time_info_t time;
  /* Loop-read to ignore missing data */
  do {
    time = c->time;
    ret = cluster_prepare_step(c, time);
    TIMEADD(c->time, c->g, 1);
  }while(!ret);
  
  if(ret < 0) /* EOF */
    goto out;
  
  /* Execute if possible */
  ret = cluster_execute_step(c, time);
  
 out:
  return ret;
}

int cluster_run(struct cluster *c) {

  time_info_t time;
  struct timeline *t;

  /* TODO : Set time_min & time_max in a realistic way */
  TIME_FOR_EACH(TIME_MIN, TIME_MAX, GRANULARITY_DAY, time){
    /* Passing time 'by hand" */
    if(cluster_prepare_step(c, time) != -1)
      cluster_execute_step(c, time);
  }
  
  return 0;
}
