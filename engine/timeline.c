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

  /* TODO : Don't forget to free() & unload data */
  timeline_entry_free(&t->list_entry);
  t->cache = &t->list_entry;
}

/* TODO : Create loader object/superclass ? */

int timeline_load(struct timeline *t, struct input *in) {
  
  int n;
  
  for(n = 0;; n++){
    struct candle *candle = malloc(sizeof *candle);
    int eof = input_read(in, __timeline_entry__(candle));
    if(eof != -1){
      __list_add_tail__(&t->list_entry,
			__timeline_entry__(candle));
      continue;
    }
    
    free(candle);
    break;
  }
  
  return n;
}

int timeline_step(struct timeline *t, struct input *in) {

  struct candle *candle = malloc(sizeof *candle);
  int eof = input_read(in, __timeline_entry__(candle));
  if(eof != -1){
    __list_add_tail__(&t->list_entry,
		      __timeline_entry__(candle));
    return 1;
  }

  free(candle);
  return -1;
}
