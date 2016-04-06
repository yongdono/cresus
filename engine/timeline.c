/*
 * Cresus EVO - timeline_entry.c 
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 * 
 */

#include <stdlib.h>
#include "timeline.h"

int timeline_init(struct timeline *t, granularity_t g, struct input *in) {
  
  timeline_entry_init(&t->list_entry, NULL, 0); /* FIXME ? */
  t->cache = &t->list_entry;
  t->g = g;
  
  return 0;
}

void timeline_free(struct timeline *t) {
  
  timeline_entry_free(&t->list_entry);
  t->cache = &t->list_entry;
}

#if 0

/* TODO : Create loader object/superclass */

int timeline_load(struct timeline *t, struct input *in) {
  
  int n = 0;
  struct candle candle;

  /* TODO : Make generic ? */
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
#endif

struct timeline_entry *timeline_by_date(struct timeline *t, time_t time) {

  struct list *ptr;
  struct timeline_entry *entry;
  time_t tm = timeline_entry_timecmp(t->cache, time, t->g);

  if(!tm)
    /* time is the same */
    goto out;
  
  if(tm < 0){
    /* time is forward */
    __list_for_each__(&__list__(t->cache), ptr, entry)
      if(!timeline_entry_timecmp(entry, time, t->g)){
	t->cache = entry;
	goto out;
      }
    
  }else{
    /* time is backwards*/
    __list_for_each_prev__(&__list__(t->cache), ptr, entry)
      if(!timeline_entry_timecmp(entry, time, t->g)){
	t->cache = entry;
	goto out;
      }
  }
  
  return NULL;
  
 out:
  return t->cache;
}
