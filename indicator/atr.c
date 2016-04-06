/*
 * Cresus EVO - atr.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <math.h>
#include <stdio.h>

#include "atr.h"

static int atr_feed(struct indicator *i, struct candle *c)
{
  struct atr *a = __indicator_self__(i);
  
  /* Compute "True Range" */
  double tr = c->high - c->low;
  double h = fabs(c->high - a->ref->close);
  double l = fabs(c->low - a->ref->close);
  
  tr = (h > tr ? h : tr);
  tr = (l > tr ? l : tr);

  /* Average it */
  a->value = (a->value * (a->period - 1) + tr) / a->period;
  
  /* Remember last candle */
  a->ref = c;
  
  return 0;
}

int atr_init(struct atr *a, int period, struct candle *seed)
{
  /* Super() */
  __indicator_super__(a, atr_feed);
  __indicator_set_string__(a, "atr[%d]", period);
  
  a->ref = seed;
  a->period = period;

  /* First seed */
  a->value = seed->high - seed->low;
  
  return 0;
}

void atr_free(struct atr *a)
{
  __indicator_free__(a);
}

double atr_value(struct atr *a)
{
  return a->value;
}
