/*
 * Cresus EVO - candle.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "candle.h"
#include "framework/verbose.h"

int candle_init(struct candle *ctx,
		time_info_t time,
                granularity_t g,
		double open, double close,
		double high, double low,
		double volume)
{  
  /* superclass */
  __timeline_entry_super__(ctx, time, g);
  
  /* Set */
  ctx->open = open;
  ctx->close = close;
  ctx->high = high;
  ctx->low = low;
  ctx->volume = volume;
  
  return 0;
}

void candle_release(struct candle *ctx)
{  
  __timeline_entry_release__(ctx);
}

void candle_merge(struct candle *ctx, struct candle *c2)
{
  ctx->open += c2->open;
  ctx->close += c2->close;
  ctx->high += c2->high;
  ctx->low += c2->low;
  ctx->volume += c2->volume;
}

double candle_get_value(struct candle *ctx, candle_value_t value)
{  
  switch(value) {
  case CANDLE_OPEN : return ctx->open;
  case CANDLE_CLOSE : return ctx->close;
  case CANDLE_HIGH : return ctx->high;
  case CANDLE_LOW : return ctx->low;
  case CANDLE_VOLUME : return ctx->volume;
  case CANDLE_TYPICAL : return (ctx->high + ctx->low + ctx->close) / 3.0;
  case CANDLE_WEIGHTED : return (ctx->high + ctx->low + 2 * ctx->close) / 4.0;
  case CANDLE_MEDIAN : return (ctx->high + ctx->low) / 2.0;
  case CANDLE_TOTAL : return (ctx->high + ctx->low + ctx->open + ctx->close) / 4.0;
  }
  
  /* Unknown */
  PR_WARN("bad value requested (%d)", value);
  return 0.0;
}

int candle_get_direction(struct candle *ctx)
{
  return (ctx->close - ctx->open);
}

/* Debug */

static char str[256];

const char *candle_str(struct candle *ctx)
{
  return candle_str_r(ctx, str);
}

const char *candle_str_r(struct candle *ctx, char *buf)
{
  sprintf(buf, "%s o%.2f c%.2f h%.2f l%.2f v%.0f",
	  __timeline_entry_str__(ctx), ctx->open, ctx->close,
	  ctx->high, ctx->low, ctx->volume);
  
  return buf;
}
