/*
 * Cresus EVO - bollinger.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <stdio.h>
#include "bollinger.h"

static int bollinger_feed(struct indicator *i, struct timeline_entry *e)
{  
  struct bollinger *b = (void*)i;
  struct candle *c = (void*)e;
  double value = candle_get_value(c, b->cvalue);
  b->value.mma = average_update(&b->avg, value);

  if(average_is_available(&b->avg)){
    double stddev = average_stddev(&b->avg);
    b->value.hi = b->value.mma + b->stddev_factor * stddev;
    b->value.lo = b->value.mma - b->stddev_factor * stddev;
  }
  
  return (b->value.mma != 0.0 ? 1 : 0);
}

static void bollinger_reset(struct indicator *i)
{
  struct bollinger *b = (void*)i;
  average_reset(&b->avg);
}

int bollinger_init(struct bollinger *b, unique_id_t id, int period,
                   double stddev_factor, candle_value_t cvalue)
{  
  /* init */
  __indicator_init__(b, id, bollinger_feed, bollinger_reset);
  __indicator_set_string__(b, "boll[%d, %.1f]", period, stddev_factor);
  
  b->stddev_factor = stddev_factor;
  b->cvalue = cvalue;
  average_init(&b->avg, AVERAGE_MATH, period);
  
  return 0;
}

void bollinger_release(struct bollinger *b)
{
  __indicator_release__(b);
  average_release(&b->avg);
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
