/*
 * Cresus EVO - mobile.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mobile.h"

#if 0
static void mobile_manage_direction(struct mobile *ctx, double avg,
                                    struct candle *candle)
{  
  /* Check direction change */
  if(avg > ctx->avg.value){
    if(ctx->dir == MOBILE_DIR_DOWN)
      __indicator_set_event__(m, candle, MOBILE_EVENT_CHDIR_UP);
    
    ctx->dir = MOBILE_DIR_UP;
    
  }else if(avg < ctx->avg.value){
    if(ctx->dir == MOBILE_DIR_UP)
      __indicator_set_event__(m, candle, MOBILE_EVENT_CHDIR_DOWN);
    
    ctx->dir = MOBILE_DIR_DOWN;
  }
  
  /* If equals, keep last info */
}

static void mobile_manage_position(struct mobile *ctx, double avg,
                                   struct candle *candle) {
  
  double value = candle_get_value(candle, ctx->cvalue);
  if(avg > value){
    if(ctx->pos == MOBILE_POS_BELOW)
      __indicator_set_event__(m, candle, MOBILE_EVENT_CROSSED_DOWN);
    
    ctx->pos = MOBILE_POS_ABOVE;
    
  }else if(avg < value) {
    if(ctx->pos == MOBILE_POS_ABOVE)
      __indicator_set_event__(m, candle, MOBILE_EVENT_CROSSED_UP);
    
    ctx->pos = MOBILE_POS_BELOW;
  }
  
  /* If equals, do nothing */
}
#endif

static int mobile_feed(struct indicator *i, struct timeline_entry *e) {
  
  struct mobile_entry *entry;
  struct mobile *ctx = (void*)i;
  struct candle *c = (void*)e;
  /* Trying to get average values */
  double last_avg = average_value(&ctx->avg);
  double avg = average_update(&ctx->avg, candle_get_value(c, ctx->cvalue));
  
  if(average_is_available(&ctx->avg)){
    /* Create new entry */
    if(mobile_entry_alloc(entry, i, avg, (avg - last_avg))){
      timeline_entry_add_indicator_entry(e, __indicator_entry__(entry));
      return 1;
    }
  }
  
  return 0;
}

static void mobile_reset(struct indicator *i) {

  struct mobile *ctx = (void*)i;
  average_reset(&ctx->avg);
}

int mobile_init(struct mobile *ctx, unique_id_t id, mobile_t type,
		int period, candle_value_t cvalue) {
  
  /* Super */
  __indicator_init__(ctx, id, mobile_feed, mobile_reset);
  __indicator_set_string__(ctx, "%cma[%d]",
			   ((type == MOBILE_EMA) ? 'e' : 'm'),
			   period);

  ctx->type = type;
  ctx->cvalue = cvalue;

  switch(ctx->type) {
  case MOBILE_MMA : average_init(&ctx->avg, AVERAGE_MATH, period); break;
  case MOBILE_EMA : average_init(&ctx->avg, AVERAGE_EXP, period); break;
  }
  
  return 0;
}

void mobile_release(struct mobile *ctx)
{
  __indicator_release__(ctx);
  average_release(&ctx->avg);
}

double mobile_average(struct mobile *ctx)
{
  return average_value(&ctx->avg);
}

double mobile_stddev(struct mobile *ctx)
{
  return average_stddev(&ctx->avg);
}
