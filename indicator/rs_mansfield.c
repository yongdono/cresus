/*
 * Cresus EVO - rs_mansfield.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "rs_mansfield.h"

int rs_mansfield_init(struct rs_mansfield *r, int period,
		      const struct candle *seed,
		      const struct candle *seed_index) {

  __indicator_super__(r, CANDLE_CLOSE, rs_mansfield_feed);
  __indicator_set_string__(r, "rsm[%d]", period);

  average_init(&r->mma, AVERAGE_MATH, period, 0.0); /* FIXME */
  return 0;
}

void rs_mansfield_free(struct rs_mansfield *r) {

  __indicator_free__(r);
  average_free(&r->mma);
}

int rs_mansfield_feed(struct indicator *i, const struct candle *seed) {

  /* RSM = ((RSD(today) / mma(RSD(today), n)) - 1) * 100.0 */
  return -1;
}

/* Indicator-specific */
double rs_mansfield_value(struct rs_mansfield *r) {

  return average_value(&r->mma);
}
