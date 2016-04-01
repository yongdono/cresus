#ifndef RS_DORSEY_H
#define RS_DORSEY_H

/*
 * Dorsey Relative Strength
 * Very simple formula :
 * RSD = (close / close_index) * 100.0
 */

#include "framework/indicator.h"

struct rs_dorsey {
  /* As always, inherits from indicator */
  struct indicator parent;
  double value;
};

int rs_dorsey_init(struct rs_dorsey *r, const struct candle *seed,
		   const struct candle *seed_index);

void rs_dorsey_free(struct rs_dorsey *r);

int rs_dorsey_feed(struct indicator *i, const struct candle *c);

/* Indicator-specific */
double rs_dorsey_value(struct rs_dorsey *r);

#endif
