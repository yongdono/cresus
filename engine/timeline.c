/*
 * Cresus EVO - timeline_entry.c 
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 * 
 */

#include <stdlib.h>

#include "candle.h"
#include "timeline.h"

int timeline_init(struct timeline *t) {
  
  __list_head_init__(&t->list_entry);
  __slist_head_init__(&t->slist_indicator);
  t->cache = NULL;
    
  return 0;
}

void timeline_free(struct timeline *t) {

  /* 
   * TODO : Don't forget to free() & unload data 
   * list_entry AND slist_indicator
   */
  //timeline_entry_free(&t->list_entry);
  //indicator_free(&t->slist_indicator);
  t->cache = NULL;
}

int timeline_add_indicator(struct timeline *t, struct indicator *i) {

  slist_insert(&t->slist_indicator, __slist__(i));
}

int timeline_step(struct timeline *t, struct input *in) {

  struct indicator *indicator;
  struct timeline_entry *entry = input_read(in);
  
  if(entry != NULL){
    /* Cache data */
    list_add_tail(&t->list_entry, __list__(entry));
    /* Debug */
    printf("%s\n", candle_str(__timeline_entry_self__(entry)));
    /* Run indicators */
    __slist_for_each__(&t->slist_indicator, indicator)
      indicator_feed(indicator, __timeline_entry_self__(entry));
  }

  return (entry ? 0 : -1);
}

int timeline_load(struct timeline *t, struct input *in) {
  
  int n;
  
  for(n = 0;; n++)
    if(timeline_step(t, in) < 0)
      break;
  
  return n;
}

