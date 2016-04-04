/*
 * Cresus EVO - rsi.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <math.h>
#include <stdlib.h>

#include "rsi.h"
#include "mobile.h"

int rsi_init(struct rsi *r, int period, const struct candle *c)
{
  /* Super() */
  __indicator_super__(r, CANDLE_CLOSE, rsi_feed);
  __indicator_set_string__(r, "rsi[%d]", period);
  
  r->value = 0.0;
  r->last = candle_get_value(c, __indicator_candle_value__(r));
  
  average_init(&r->h, AVERAGE_EXP, period, 0);
  average_init(&r->b, AVERAGE_EXP, period, 0);
  
  return 0;
}

void rsi_free(struct rsi *r)
{
  __indicator_free__(r);
  average_free(&r->h);
  average_free(&r->b);
}

int rsi_feed(struct indicator *i, const struct candle *c)
{
  double h, b;
  struct rsi *r = __indicator_self__(i);
  double diff = candle_get_value(c, i->value) - r->last;

  /* RSI formula :
   * 100.0 - (100.0 / (1 + h / b))
   * or
   * (h / (h + b)) * 100.0
   * where h is the ema of ups of last n days
   * and b is the fabs ema of downs of last n days
   */
  if(diff > 0) h = average_update(&r->h, diff);
  if(diff < 0) b = average_update(&r->b, fabs(diff));
  /* Compute RSI the esay way */
  r->value = (h / (h + b)) * 100.0;

  /* TODO : add event management */
  r->last = candle_get_value(c, i->value);
  return 0;
}

double rsi_value(struct rsi *r) {

  return r->value;
}
