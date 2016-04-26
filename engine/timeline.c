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
#include "framework/verbose.h"

int timeline_init(struct timeline *t, const char *name, struct input *in) {
  
  /* Inheritance */
  __slist_super__(t);
  /* Data */
  strncpy(t->name, name, sizeof(t->name));
  t->ref = &t->list_entry;
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
    t->ref = entry; /* Speed up things */
  }
  
  return entry;
}

int timeline_entry_by_time(struct timeline *t, time_info_t time,
			   struct timeline_entry **ret) {

  do {
    struct timeline_entry *entry = t->ref;
    /* Granularity is provided by input entry, beware */
    int res = timeline_entry_timecmp(entry, time);
    if(!res){
      /* Cache data. Is that the right place ? */
      list_add_tail(&t->list_entry, __list__(entry));
      *ret = entry;
      return 1;
      
    }else{
      if(res < 0)
	/* We're late, let's move on */
	continue;
      
      else{
	/* Didn't find any with this timecode */
	PR_WARN("no input data matched timecode %llx\n", time);
	return 0;
      }
    }
    
  }while((t->ref = input_read(t->in)));

  PR_DBG("no more data in input, it's over\n");
  return -1;
}

static void timeline_step_entry(struct timeline *t,
				struct timeline_entry *entry) {

  struct indicator *indicator;
  /* Parse indicators */
  __slist_for_each__(&t->slist_indicator, indicator){
    indicator_feed(indicator, entry);
    /* printf("indicator %s\n", indicator->str); */
  }
}

struct timeline_entry *timeline_step(struct timeline *t) {
  
  struct timeline_entry *entry = t->ref;
  if(entry != NULL){
    timeline_step_entry(t, t->ref);
    /* Increment. Beware ? */
    //t->ref = __list_self__(__list__(t->ref)->next);
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

