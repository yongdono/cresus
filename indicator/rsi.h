/*
 * Cresus EVO - rsi.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef RSI_H
#define RSI_H

/*
 * Relative Strength Index
 * Momentum indicator
 * Typical period is 12
 */

#include "math/average.h"
#include "framework/candle.h"
#include "framework/indicator.h"

struct rsi {
  /* Inherits from indicator */
  __inherits_from_indicator__;
  
  /* https://fr.wikipedia.org/wiki/Relative_strength_index */
  struct average h;
  struct average b;
  double last;

  double value;
};

int rsi_init(struct rsi *r, int period, const struct candle *c);
void rsi_free(struct rsi *r);

int rsi_feed(struct indicator *i, const struct candle *c);

/* Indicator-specific */
double rsi_value(struct rsi *r);

#endif
