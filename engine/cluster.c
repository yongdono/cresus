/*
 * Cresus EVO - cluster.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 22/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "cluster.h"
#include "candle.h"

int cluster_init(struct cluster *c, const char *name) {

  /* Super */
  __timeline_super__(c, name, NULL);
  __slist_head_init__(&c->slist_timeline);
  /* Set options */
  c->ref = &__timeline__(c)->list_entry;
  
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

int cluster_step(struct cluster *c) {

  struct timeline *t;
  /* First create cluster index */
  __slist_for_each__(&c->slist_timeline, t){
    struct timeline_entry *entry;
    if((entry = timeline_next_entry(t)))
      cluster_create_index(c, __timeline_entry_self__(entry));
  }
  
  /* Then execute indicators */
  __slist_for_each__(&c->slist_timeline, t){
    /* Step all timelines */
    struct timeline_entry *entry;
    if((entry = timeline_step(t))){
      struct indicator_entry *indicator;
      struct candle *candle = __timeline_entry_self__(entry);
      /* Indicators management */
      __slist_for_each__(&candle->slist_indicator, indicator){
	/* TODO : Use function pointer ? */
      }
    }
  }
  
  return 0;
}

int cluster_run(struct cluster *c) {

  time_info_t time;
  struct timeline *t;
  
  TIME_FOR_EACH(TIME_MIN, TIME_MAX, GRANULARITY_DAY, time){
    __slist_for_each__(&c->slist_timeline, t){
      struct timeline_entry *entry;
      if((entry = timeline_entry_by_time(t, time)))
	cluster_create_index(c, __timeline_entry_self__(entry));
      
      else{
	/* Eliminate last ref cause it's incomplete */
	struct __list__ *l = c->ref;
	entry = __list_self__(c->ref);
	c->ref = c->ref->prev; /* FIXME ? */
	/* Free */
	list_del(l);
	candle_free(__timeline_entry_self__(entry));
	fprintf(stderr, "No data available for %ll\n", time);
      }
    }
  }
}
