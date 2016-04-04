/*
 * Cresus EVO - atr.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "atr.h"

int atr_init(struct atr *a, int period, const struct candle *c)
{
  /* Super() */
  __indicator_super__(a, CANDLE_CLOSE, atr_feed);
  __indicator_set_string__(a, "atr[%d]", period);
  
  a->period = period;
  a->value = c->high - c->low;
  memcpy(&a->last, c, sizeof *c);
  
  return 0;
}

void atr_free(struct atr *a)
{
  __indicator_free__(a);
}

int atr_feed(struct indicator *i, const struct candle *c)
{
  struct atr *a = __indicator_self__(i);
  
  /* Compute "True Range" */
  double tr = c->high - c->low;
  double h = fabs(c->high - a->last.close);
  double l = fabs(c->low - a->last.close);
  
  tr = (h > tr ? h : tr);
  tr = (l > tr ? l : tr);

  /* Average it */
  a->value = (a->value * (a->period - 1) + tr) / a->period;
  
  /* Remember last candle */
  memcpy(&a->last, c, sizeof *c);
  return 0;
}

double atr_value(struct atr *a)
{
  return a->value;
}
