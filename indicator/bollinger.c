/*
 * Cresus EVO - bollinger.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <stdio.h>
#include "bollinger.h"

int bollinger_init(struct bollinger *b, int period,
                   double stddev_factor,
                   candle_value_t value,
                   struct candle *candle)
{
  /* super */
  __indicator_super__(b, value, bollinger_feed);
  __indicator_set_string__(b, "boll[%d, %.1f]", period, stddev_factor);
  
  average_init(&b->avg, AVERAGE_MATH, period,
	       candle_get_value(candle, value));
  
  b->stddev_factor = stddev_factor;
  return 0;
}

void bollinger_free(struct bollinger *b)
{
  __indicator_free__(b);
  average_free(&b->avg);
}

int bollinger_feed(struct indicator *i, const struct candle *candle)
{
  struct bollinger *b = __indicator_self__(i);
  b->value.mma = average_update(&b->avg, candle_get_value(candle, i->value));
  
  double stddev = average_stddev(&b->avg);
  b->value.hi = b->value.mma + b->stddev_factor * stddev;
  b->value.lo = b->value.mma - b->stddev_factor * stddev;
  
  return (b->value.mma != 0.0 ? 1 : 0);
}

struct bollinger_value *bollinger_get_value(struct bollinger *b)
{
  return (b->value.mma != 0.0 ? &b->value : NULL);
}

const char *bollinger_str(struct bollinger *b)
{
  sprintf(b->str, "%.2lf %.2lf:%.2lf", b->value.mma,
	  b->value.hi, b->value.lo);

  return b->str;
}
