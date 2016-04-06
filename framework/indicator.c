/*
 * Cresus EVO - indicator.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/20/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#include "indicator.h"

int indicator_init(struct indicator *i, void *self, candle_value_t value,
                   indicator_feed_ptr feed) {
  
  /* Super */
  __slist_super__(i);
  __indicator_self__(i) = self;
  
  i->value = value;
  i->feed = feed;
  
  return 0;
}

void indicator_free(struct indicator *i) {

  __slist_free__(i);
  i->feed = NULL;
}

void indicator_set_event(struct indicator *i, struct candle *candle,
			 int event) {
  
  /* Nothing to do now */
  /* Set in candle or timeline maybe */
}
