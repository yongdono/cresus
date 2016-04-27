/*
 * Cresus EVO - cluster.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 22/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "candle.h"
#include "cluster.h"
#include "framework/verbose.h"

int cluster_init(struct cluster *c, const char *name,
		 time_info_t time_min, time_info_t time_max) {

  /* Super */
  __timeline_super__(c, name, NULL);
  __slist_head_init__(&c->slist_timeline);
  /* Set options */
  c->ref = &__timeline__(c)->list_entry;
  calendar_init(&c->cal, time_min, GRANULARITY_DAY); /* FIXME */
  c->time_max = time_max;

  return 0;
}

void cluster_release(struct cluster *c) {

  __timeline_release__(c);
  __slist_head_release__(&c->slist_timeline);

  c->ref = NULL;
  calendar_release(&c->cal);
}

int cluster_add_timeline(struct cluster *c, struct timeline *t) {

  __slist_insert__(&c->slist_timeline, t);
  return 0;
}

static int cluster_prepare_step(struct cluster *c, time_info_t time,
				struct timeline_entry **ret) {
  
  int res;
  char buf[256]; /* debug */
  struct timeline *t;
  struct candle *candle;
  
  if(!candle_alloc(candle, time, c->cal.g, 0, 0, 0, 0, 0)) /* FIXME */
    return -1; /* Error. TODO : add some kind of errno */
  
  __slist_for_each__(&c->slist_timeline, t){
    struct timeline_entry *entry;
    /* Why not use granularity here to merge candles in timeline object ? */
    if((res = timeline_entry_by_time(t, time, &entry)) <= 0){
      PR_WARN("not enough data available in %s for %s\n", t->name,
	      time2str(time, entry->granularity, buf));
      
      candle_free(candle);
      return res; /* EOF or 0 (no data available) */
      
    }else{
      /* Merge candles */
      struct candle *c2 = __timeline_entry_self__(entry);
      candle_merge(candle, c2);
      PR_INFO("added candle %s %s in cluster\n", t->name,
	      timeline_entry_str(entry, buf, sizeof buf));
    }
  }
  
  /* Add data to list */
  *ret = __timeline_entry__(candle);
  __list_add_tail__(&__timeline__(c)->list_entry,
		    __timeline_entry__(candle));
  
  return 1;
}

static int cluster_execute_step(struct cluster *c,
				struct timeline_entry *entry) {

  struct timeline *t;
  /* Execute indicators */
  __slist_for_each__(&c->slist_timeline, t){
    /* Step all timelines */
    if(timeline_step(t, entry)){
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
  struct timeline_entry *entry;
  time_info_t time = calendar_time(&c->cal);
  
  /* Loop-read to ignore missing data */
  do {
    ret = cluster_prepare_step(c, time, &entry);
    calendar_next(&c->cal, &time);
  }while(!ret);
  
  if(ret < 0){ /* EOF */
    PR_ERR("no more data in input(s)\n");
    goto out;
  }
  
  /* Execute when possible */
  ret = cluster_execute_step(c, entry);
  
 out:
  return ret;
}

int cluster_run(struct cluster *c) {

  time_info_t time;
  struct timeline *t;
  struct timeline_entry *entry;
  
  /* TODO : Set time_min & time_max in a realistic way */
  TIME_FOR_EACH(TIME_MIN, TIME_MAX, GRANULARITY_DAY, time){
    /* Passing time 'by hand" */
    if(cluster_prepare_step(c, time, &entry) != -1)
      cluster_execute_step(c, entry);
    
    else
      break;
  }
  
  return 0;
}
