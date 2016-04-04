/*
 * Cresus EVO - rs_dorsey.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "rs_dorsey.h"

int rs_dorsey_init(struct rs_dorsey *r, const struct candle *seed,
		   const struct candle *seed_index) {

  /* super() */
  __indicator_super__(r, CANDLE_CLOSE, rs_dorsey_feed);
  return 0;
}

void rs_dorsey_free(struct rs_dorsey *r) {

  __indicator_free__(r);
}

int rs_dorsey_feed(struct indicator *i, const struct candle *c) {

  struct rs_dorsey *r = __indicator_self__(i);
  /*
   * TODO : Find a way to push 2 streams of candles in this indicator
   */
  
  return -1;
}

/* Indicator-specific */
double rs_dorsey_value(struct rs_dorsey *r) {

  return r->value;
}
