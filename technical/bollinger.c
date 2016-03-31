#include <stdio.h>
#include "bollinger.h"

int bollinger_init(struct bollinger *b, int period,
                   double stddev_factor,
                   candle_value_t value,
                   struct candle *candle)
{
  /* super */
  indicator_init(&b->parent, value, bollinger_feed);
  
  b->stddev_factor = stddev_factor;
  return mobile_init(&b->mma, MOBILE_MMA, period, value, candle);
}

void bollinger_free(struct bollinger *b)
{
  mobile_free(&b->mma);
}

int bollinger_feed(struct indicator *i, const struct candle *candle)
{
  struct bollinger *b = (struct bollinger*)i;
  b->value.mma = mobile_feed(&b->mma.parent, candle);
  
  double stddev = mobile_stddev(&b->mma);
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
