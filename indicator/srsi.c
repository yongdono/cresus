/*
 * Cresus EVO - srsi.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 11/20/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#include <stdlib.h>

#include "srsi.h"

int srsi_init(struct srsi *s, int max, const struct candle *seed) {
  
  /* super() */
  __indicator_super__(s, CANDLE_CLOSE, srsi_feed);
  
  s->len = 0;
  s->max = max;
  
  if((s->array = malloc(sizeof(*s->array) * max)))
    return -1;
  
  /* Init */
  srsi_feed(&__indicator__(s), seed);
  return 0;
}

void srsi_free(struct srsi *s) {
  
  __indicator_free__(s);
  free(s->array);
}

int srsi_feed(struct indicator *i, const struct candle *candle) {
  
  struct srsi *s = __indicator_self__(i);
  int start = (candle->open < candle->close ? candle->open : candle->close);
  int end = (candle->close < candle->open ? candle->open : candle->close);
  
  for(int i = start; i <= end; i++){
    if(candle->close >= candle->open)
      s->array[i].bull++;
    else
      s->array[i].bear++;
    
    s->array[i].total++;
  }
  
  s->len++;
  return 0;
}
