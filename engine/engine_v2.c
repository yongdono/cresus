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
  ctx->start_time = TIME64_MIN;
  ctx->end_time = TIME64_MAX;
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

void engine_v2_run(struct engine_v2 *ctx, struct engine_v2_interface *i)
{
  struct timeline_slice *slice;
  struct indicator_n3 *indicator_n3;
  struct timeline_track_n3 *track_n3;

  /* Run all slices */
  __list_for_each__(&ctx->timeline->by_slice, slice){
    
    /* We MUST stop at end_time */
    if(TIME64CMP(slice->time, ctx->end_time, GR_DAY) > 0)
      break;
    
    /* Run "new" slice */
    if(i->feed_slice)
      i->feed_slice(ctx, slice);
    
    /* Run "new" track */
    timeline_slice_for_each_track_n3(slice, track_n3){
      if(i->feed_track_n3)
        i->feed_track_n3(ctx, slice, track_n3);
      
      /* Run "new" indicators */
      timeline_track_n3_for_each_indicator_n3(track_n3, indicator_n3){
        if(i->feed_indicator_n3)
          i->feed_indicator_n3(ctx, track_n3, indicator_n3);
      }
    }
    
    /* Post-processing */
    if(i->post_slice)
      i->post_slice(ctx, slice);
  }
}
