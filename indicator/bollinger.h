/*
 * Cresus EVO - bollinger.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef BOLLINGER_H
#define BOLLINGER_H

#include "framework/indicator.h"
#include "framework/candle.h"
#include "mobile.h"

struct bollinger_value {
  double mma;
  double hi, lo;
};

struct bollinger {
  /* Inherits from indicator */
  struct indicator parent;
  
  struct average avg;
  double stddev_factor;
  
  /* Indicator-specific */
  struct bollinger_value value;
  char str[32]; /* FIXME */
};

int bollinger_init(struct bollinger *b, int period, double stddev_factor,
                   candle_value_t value, struct candle *candle);

void bollinger_free(struct bollinger *b);

int bollinger_feed(struct indicator *i, const struct candle *candle);

/* Indicator-specific */
struct bollinger_value *bollinger_get_value(struct bollinger *b);
const char *bollinger_str(struct bollinger *b);

#endif
