/*
 * Cresus EVO - timeline_entry.c 
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 * 
 */

#include <stdlib.h>
#include "timeline.h"

int timeline_init(struct timeline *t, int granularity, struct input *in) {
  
  timeline_entry_init(&t->list_entry, NULL, 0);
  t->granularity = granularity;
  t->cache = &t->list_entry;
  
  return 0;
}

void timeline_free(struct timeline *t) {
  
  timeline_entry_free(&t->list_entry);
  t->cache = &t->list_entry;
}

int timeline_load(struct timeline *t, struct input *in) {
  
  int n = 0;
  struct candle candle;

  /* TODO ; Make generic ? */
  while(in->read(in, &candle) != -1){
    struct candle *entry;
    if((entry = malloc(sizeof *entry))){
      /* Add it to the list */
      __list_add_tail__(&__list__(&t->list_entry),
			&__timeline_entry__(entry));
      n++;
    }
  }
  
  return n;
}

struct timeline_entry *timeline_by_date(struct timeline *t, time_t time) {

  struct list *ptr;
  struct timeline_entry *entry;
  time_t diff = timeline_entry_difftime(t->cache, time);

  /* TODO : What about granularity ? Something's missing */
  
  if(!diff)
    /* time is the same */
    goto out;
  
  if(diff < 0){
    /* time is forward */
    __list_for_each__(&__list__(t->cache), ptr, entry)
      if(timeline_entry_difftime(entry, time) < t->granularity){
	t->cache = entry;
	goto out;
      }
    
  }else{
    /* time is backwards*/
    __list_for_each_prev__(&__list__(t->cache), ptr, entry)
      if(timeline_entry_difftime(entry, time) < t->granularity){
	t->cache = entry;
	goto out;
      }
  }
  
  return NULL;
  
 out:
  return t->cache;
}