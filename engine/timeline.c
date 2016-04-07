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
  
  timeline_entry_init(&t->list_entry, 0, 0); /* FIXME ? */
  t->cache = &t->list_entry;

  indicator_init(&t->slist_indicator, NULL);
  
  return 0;
}

void timeline_free(struct timeline *t) {

  /* 
   * TODO : Don't forget to free() & unload data 
   * list_entry AND slist_indicator
   */
  timeline_entry_free(&t->list_entry);
  indicator_free(&t->slist_indicator);
  t->cache = &t->list_entry;
}

int timeline_add_indicator(struct timeline *t, struct indicator *i) {

  slist_insert(__slist__(&t->slist_indicator), __slist__(i));
}

int timeline_step(struct timeline *t, struct input *in) {

  struct indicator *indicator;
  struct candle *candle = malloc(sizeof *candle);
  int eof = input_read(in, __timeline_entry__(candle));
  
  if(eof != -1){
    /* Cache data */
    __list_add_tail__(__list__(&t->list_entry),
		      __timeline_entry__(candle));
    /* Run indicators */
    __slist_for_each__(__slist__(&t->slist_indicator), indicator)
      indicator_feed(indicator, candle);
    
    return 1;
  }

  free(candle);
  return eof;
}

int timeline_load(struct timeline *t, struct input *in) {
  
  int n;
  
  for(n = 0;; n++)
    if(timeline_step(t, in) < 0)
      break;
  
  return n;
}

