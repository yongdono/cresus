#ifndef RS_MANSFIELD_H
#define RS_MANSFIELD_H

/*
 * Relative Strength (Mansfield)
 * Fomula :
 * RSM = ((RSD(today) / mma(RSD(today), n)) - 1) * 100.0
 */

#include "math/average.h"
#include "framework/indicator.h"

struct rs_mansfield {
  /* As always, inherits from indicator */
  struct indicator parent;
  struct average mma;
};

int rs_mansfield_init(struct rs_mansfield *r, const struct candle *seed,
		   const struct candle *seed_index);

void rs_mansfield_free(struct rs_mansfield *r);

int rs_mansfield_feed(struct indicator *i, const struct candle *seed);

/* Indicator-specific */
double rs_mansfield_value(struct rs_mansfield *r);

#endif
