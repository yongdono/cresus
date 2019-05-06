/*
 * Cresus CommonTL - engine_v2.c
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 06/05/2019
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#include "engine_v2.h"

int engine_v2_init(struct engine_v2 *ctx, struct timeline *t)
{
  /* Timeline */
  ctx->timeline = t;
  /* Time */
  ctx->start_time = TIME_MIN;
  ctx->end_time = TIME_MAX;
  /* Fees */
  ctx->transaction_fee = 0;
  /* Output */
  ctx->csv_output = 0;
    
  return 0;
}

void engine_v2_release(struct engine_v2 *ctx)
{
}

int engine_v2_set_common_opt(struct engine_v2 *ctx, struct common_opt *opt)
{
  if(opt->start_time.set) ctx->start_time = opt->start_time.t;
  if(opt->end_time.set) ctx->end_time = opt->end_time.t;
  if(opt->transaction_fee.set) ctx->transaction_fee = opt->transaction_fee.d;
  if(opt->csv_output.set) ctx->csv_output = opt->csv_output.i;
  
  return 0;
}

void engine_v2_run(struct engine_v2 *ctx, engine_v2_feed_ptr feed)
{
  struct timeline_slice *slice;
  __list_for_each__(&ctx->timeline->by_slice, slice){
    /* We MUST stop at end_time */
    if(TIMECMP(slice->time, ctx->end_time, GR_DAY) > 0)
      break;

    /* Run custom routines */
    feed(ctx, slice);
  }
}
