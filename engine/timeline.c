/*
 * Cresus EVO - timeline_entry.c 
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 * 
 */

#include <stdlib.h>
#include <string.h>
#include "timeline.h"

int timeline_init(struct timeline *t, const char *name, struct input *in) {
  
  /* Inheritance */
  __slist_super__(t);
  /* Data */
  strncpy(t->name, name, sizeof(t->name));
  t->current_timeline_entry = NULL;
  t->in = in;
  /* Internals */
  __list_head_init__(&t->list_entry);
  __slist_head_init__(&t->slist_indicator);
  
  return 0;
}

void timeline_release(struct timeline *t) {

  __list_head_release__(&t->list_entry);
  __slist_head_release__(&t->slist_indicator);
}

int timeline_add_indicator(struct timeline *t, struct indicator *i) {

  __slist_insert__(&t->slist_indicator, i);
}

struct timeline_entry *timeline_next_entry(struct timeline *t) {
  
  struct timeline_entry *entry;
  if((entry = input_read(t->in))){
    /* Cache data */
    list_add_tail(&t->list_entry, __list__(entry));
    t->current_timeline_entry = entry; /* Speed up things */
  }
  
  return entry;
}

struct timeline_entry *timeline_entry_by_time(struct timeline *t,
					      time_info_t time) {
  struct timeline_entry *entry;
  while((entry = input_read(t->in))){
    /* Granularity is provided by input entry, beware */
    int res = timeline_entry_timecmp(entry, time);
    if(!res){
      /* Cache data */
      list_add_tail(&t->list_entry, __list__(entry));
      t->current_timeline_entry = entry; /* Speed up things */
      break;
      
    }else{
      if(res < 0) continue; /* We're late, let's move on */
      else return NULL; /* Didn't find any with this timecode */
    }
  }
  
  return entry;
}

static void timeline_step_entry(struct timeline *t) {

  struct indicator *indicator;
  /* Parse indicators */
  __slist_for_each__(&t->slist_indicator, indicator){
    indicator_feed(indicator, t->current_timeline_entry);
    /* printf("indicator %s\n", indicator->str); */
  }
}

struct timeline_entry * timeline_step(struct timeline *t) {

  struct timeline_entry *entry = t->current_timeline_entry;
  if(entry != NULL){
    timeline_step_entry(t);
    t->current_timeline_entry = NULL;
  }
  
  return entry;
}

int timeline_execute(struct timeline *t) {
  
  int n;
  
  for(n = 0;; n++)
    if(timeline_step(t) == NULL)
      break;
  
  return n;
}

