/*
 * rs_dorsey.c
 * (c) 2016 Joachim Naulet
 *
 */
#include "rs_dorsey.h"

int rs_dorsey_init(struct rs_dorsey *r, const struct candle *seed,
		   const struct candle *seed_index) {

  /* super() */
  indicator_init(&r->parent, CANDLE_CLOSE, rs_dorsay_feed);
  return 0;
}

void rs_dorsey_free(struct rs_dorsey *r) {

  indicator_free(&r->parent);
}

int rs_dorsey_feed(struct indicator *i, const struct candle *c) {

  /*
   * TODO : Find a way to push 2 streams of candles in this indicator
   */
  
  return -1;
}

/* Indicator-specific */
double rs_dorsey_value(struct rs_dorsey *r) {

  return r->value;
}
