/*
 * Cresus EVO - atr.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef ATR_H
#define ATR_H

#include "framework/indicator.h"

struct atr {
  /* Parent */
  __inherits_from_indicator__;
    
  int period;
  double value;
  struct candle last;
};

int atr_init(struct atr *a, int period, const struct candle *c);
void atr_free(struct atr *a);

int atr_feed(struct indicator *i, const struct candle *c);

/* Indicator-specific */
double atr_value(struct atr *a);

#endif
