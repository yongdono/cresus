/*
 * Cresus CommonTL - engine_v2.c
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 06/05/2019
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#include "engine_v2.h"

/*
 * Orders
 */

/* Real order struct */
struct engine_v2_order {
  __inherits_from__(struct slist);
  unique_id_t track_uid;
  engine_v2_order_t type;
  engine_v2_order_by_t by;
  double value;
};

static int engine_v2_order_init(struct engine_v2_order *ctx,
				unique_id_t track_uid,
				engine_v2_order_t type,
				double value,
				engine_v2_order_by_t by)
{
  __slist_init__(ctx); /* super() */
  ctx->track_uid = track_uid;
  ctx->type = type;
  ctx->value = value;
  ctx->by = by;
  return 0;
}

static void engine_v2_order_release(struct engine_v2_order *ctx)
{
  __slist_release__(ctx);
}

#define engine_v2_order_alloc(ctx, track_uid, type, value, by)		\
  DEFINE_ALLOC(struct engine_v2_order, ctx,				\
	       engine_v2_order_init, track_uid, type, value, by)
#define engine_v2_order_free(ctx)		\
  DEFINE_FREE(ctx, engine_v2_order_release)

/* 
 * Positions
 */
struct engine_v2_position {
  __inherits_from__(struct slist_by_uid);
  double shares;
};

int engine_v2_position_init(struct engine_v2_position *ctx, unique_id_t uid)
{
  __slist_by_uid_init__(ctx, uid);
  ctx->shares = 0.0;
  return 0;
}

void engine_v2_position_release(struct engine_v2_position *ctx)
{
  __slist_by_uid_release__(ctx);
}

#define engine_v2_position_alloc(ctx, uid)				\
  DEFINE_ALLOC(struct engine_v2_position, ctx, engine_v2_position_init, uid)
#define engine_v2_position_free(ctx)					\
  DEFINE_FREE(struct engine_v2_position, ctx, engine_v2_position_release)

void engine_v2_position_buy_cash(struct engine_v2_position *ctx,
				 struct timeline_track_n3 *track_n3,
				 double value)
{
  double q = value / track_n3->open;
  ctx->shares += q;
}

void engine_v2_position_buy_shares(struct engine_v2_position *ctx,
				   struct timeline_track_n3 *track_n3,
				   double value)
{
  ctx->shares += value;
}

/*
 * Engine v2
 */

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

  /* Init lists */
  slist_head_init(&ctx->slist_orders);
  slist_head_init(&ctx->slist_positions);
  
  /* Init positions slist */
  struct timeline_track *track;
  __slist_for_each__(&t->by_track, track){
    struct engine_v2_position *p;
    engine_v2_position_alloc(p, timeline_track_uid(track));
    __slist_insert__(&ctx->slist_positions, p);
  }
  
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

static void engine_v2_run_orders(struct engine_v2 *ctx,
				 struct timeline_track_n3 *track_n3)
{
  struct engine_v2_order *order;
  __slist_for_each__(&ctx->slist_orders, order){ /* FIXME : safe? */
    /* Ignore non-track related */
    if(order->track_uid != timeline_track_n3_track_uid(track_n3))
      continue;
    
    /* Run order here */
    struct engine_v2_position *p = (struct engine_v2_position*)
      __slist_by_uid_find__(&ctx->slist_positions, order->track_uid);

    switch(order->type){
    case BUY: engine_v2_position_buy_cash(p, track_n3, order->value); break;
    case SELL: break;
    }
    
    /* Remove executed order */
    __slist_del__(order);
    engine_v2_order_free(order);
  }
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
      /* Run pending orders */
      engine_v2_run_orders(ctx, track_n3);
      
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

int engine_v2_set_order(struct engine_v2 *ctx, unique_id_t track_uid,
			engine_v2_order_t type, double value,
			engine_v2_order_by_t by)
{
  struct engine_v2_order *order;
  engine_v2_order_alloc(order, track_uid, order->type, value, by);
  if(!order) return -1;
  
  __slist_insert__(&ctx->slist_orders, order);
  return 0;
}
