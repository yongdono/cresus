/*
 * Cresus EVO - macd.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>

#include "macd.h"
#include "engine/candle.h"

static int macd_feed(struct indicator *i, struct timeline_entry *e)
{  
  struct macd *ctx = (void*)i;
  struct candle *c = (void*)e;

  /* Set 2 main averages */
  double fast = average_update(&ctx->fast, c->close);
  double slow = average_update(&ctx->slow, c->close);
  if(average_is_available(&ctx->fast) &&
     average_is_available(&ctx->slow)){
    /* Compute signal */
    double value = (fast - slow);
    double signal = average_update(&ctx->signal, value);
    if(average_is_available(&ctx->signal)){
      struct macd_entry *entry;
      if(macd_entry_alloc(entry, i, value, signal))
	timeline_entry_add_indicator_entry(e, __indicator_entry__(entry));
    }
  }
  
  /* State machine & events here */
  
  return 0;
}

static void macd_reset(struct indicator *i)
{
  struct macd *ctx = (void*)i;
  /* Reset averages */
  average_reset(&ctx->fast);
  average_reset(&ctx->slow);
  average_reset(&ctx->signal);
}

int macd_init(struct macd *ctx, unique_id_t id,
	      int fast_p, int slow_p, int signal_p)
{
  /* Super */
  __indicator_init__(ctx, id, macd_feed, macd_reset);
  __indicator_set_string__(ctx, "macd[%d,%d,%d]", fast_p, slow_p, signal_p);

  /* Seed */
  average_init(&ctx->fast, AVERAGE_EXP, fast_p);
  average_init(&ctx->slow, AVERAGE_EXP, slow_p);
  average_init(&ctx->signal, AVERAGE_EXP, signal_p);
  
  return 0;
}

void macd_release(struct macd *ctx)
{
  __indicator_release__(ctx);
  average_release(&ctx->fast);
  average_release(&ctx->slow);
  average_release(&ctx->signal);
}
