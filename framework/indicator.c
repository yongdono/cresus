/*
 * Cresus EVO - indicator.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/20/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#include "indicator.h"

int indicator_init(struct indicator *i,
                   candle_value_t value,
                   indicator_feed_ptr feed) {
  
  i->value = value;
  i->feed = feed;
  return 0;
}

void indicator_free(struct indicator *i) {
  
  i->feed = NULL;
}

int indicator_feed(struct indicator *i, const struct candle *candle) {
  
  return i->feed(i, candle);
}

void indicator_set_event(struct indicator *i, struct candle *candle,
			 int event) {
  
  /* Nothing to do now */
  /* Set in candle or timeline maybe */
}
