/*
 * Cresus EVO - engine.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 09/01/2017
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#include <math.h>
#include <stdio.h>
#include "engine.h"
#include "framework/verbose.h"

int engine_init(struct engine *ctx, struct timeline *t)
{
  ctx->timeline = t;
  /* Init lists */
  list_head_init(&ctx->list_order);
  list_head_init(&ctx->list_position_to_open);
  list_head_init(&ctx->list_position_opened);
  list_head_init(&ctx->list_position_to_close);
  list_head_init(&ctx->list_position_closed);

  /* Money */
  ctx->npos = 0;
  ctx->amount = 0;
  ctx->earnings = 0;
  ctx->fees = 0;
  ctx->balance = 0;
  /* Stats */
  ctx->nbuy = 0;
  ctx->nsell = 0;
  ctx->max_drawdown = 0;
  ctx->transaction_fee = 0;
  /* Misc */
  ctx->filter = TIME_INIT(1900, 1, 1, 0, 0, 0, 0);
  ctx->quiet = 0;
  
  return 0;
}

void engine_release(struct engine *ctx)
{
  /* Nothing to do */
  list_head_release(&ctx->list_order);
  list_head_release(&ctx->list_position_to_open);
  list_head_release(&ctx->list_position_opened);
  list_head_release(&ctx->list_position_to_close);
  list_head_release(&ctx->list_position_closed);
}

static void engine_display_order_info(struct engine *ctx,
				      struct order *o,
				      struct timeline_entry *e)
{
  struct candle *c = __timeline_entry_self__(e);
  const char *str = candle_str(c);
  
  if(ctx->quiet)
    return;

  switch(o->type){
  case ORDER_BUY:
    if(o->by == ORDER_BY_NB)
      PR_INFO("%s - Buy %.0lf securities at %.2lf VALUE\n",
	      str, o->value, c->open);
    
    if(o->by == ORDER_BY_AMOUNT)
      PR_INFO("%s - Buy %.4lf securities for %.2lf CASH\n",
	      str, o->value / c->open, o->value);
    break;

  case ORDER_SELL: /* FIXME : what if oversell ? */
    if(o->by == ORDER_BY_NB)
      PR_WARN("%s - Sell %.0lf securities at %.2lf VALUE\n",
	      str, o->value, c->open);
    
    if(o->by == ORDER_BY_AMOUNT)
      PR_WARN("%s - Sell %.4lf securities for %.2lf CASH\n",
	      str, o->value / c->open, o->value);
    break;

  case ORDER_SELL_ALL:
    PR_ERR("%s - Sell %.2lf securities for %.2lf TOTAL VALUE\n",
	   str, ctx->npos, ctx->npos * c->open);
    break;
  }
}

/* FIXME : put elsewhere */
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

static void engine_run_order(struct engine *ctx, struct order *o,
			     struct timeline_entry *e)
{
  double npos;
  struct candle *c = __timeline_entry_self__(e);

  /* display some info */
  engine_display_order_info(ctx, o, e);
  
  switch(o->type){
  case ORDER_BUY:
    if(o->by == ORDER_BY_NB) npos = o->value;
    else npos = (o->value / c->open);
    /* Buy n positions */
    ctx->npos += npos;
    ctx->amount += (npos * c->open);
    ctx->balance -= (npos * c->open);
    ctx->nbuy++;
    goto next;

  case ORDER_SELL:
    /* Warning : no negative positions ! */
    if(o->by == ORDER_BY_NB) npos = MIN(ctx->npos, o->value);
    else npos = MIN(ctx->npos, (o->value / c->open));
    /* Sell n positions */
    ctx->npos -= npos;
    ctx->earnings += (npos * c->open);
    ctx->balance += (npos * c->open);
    ctx->nsell++;
    goto next;

  case ORDER_SELL_ALL:
    ctx->earnings += (ctx->npos * c->open);
    ctx->balance += (ctx->npos * c->open);
    ctx->npos = 0;
    ctx->nsell++;
    goto next;
    
  default:
    break;
  }

  return;

 next:
  ctx->fees += ctx->transaction_fee;
  ctx->max_drawdown = MIN(ctx->balance, ctx->max_drawdown);
}

void engine_run(struct engine *ctx, engine_feed_ptr feed)
{
  struct list *safe;
  struct order *order;
  struct timeline_entry *entry;
  
  while((entry = timeline_step(ctx->timeline)) != NULL){
    struct candle *c = __timeline_entry_self__(entry); /* FIXME ? */
    /* First : check if there are opening positions */
    __list_for_each_safe__(&ctx->list_order, order, safe){
      /* Run */
      engine_run_order(ctx, order, entry);
      __list_del__(order);
    }
    
    /* Then : feed the engine */
    feed(ctx, ctx->timeline, entry);
    ctx->close = c->close;
  }
}

int engine_place_order(struct engine *ctx, order_t type,
		       order_by_t by, double value)
{
  struct order *order;
  struct timeline_entry *entry;

  if(timeline_entry_current(ctx->timeline, &entry) != -1){
    /* Filter orders if needed */
    if(TIMECMP(entry->time, ctx->filter, GRANULARITY_DAY) < 0)
      goto err;
  
    if(order_alloc(order, type, by, value)){
      __list_add_tail__(&ctx->list_order, order);
      return 0;
    }
  }

 err:
  return -1;
}

void engine_display_stats(struct engine *ctx)
{
  /* What we got left */
  double assets_value = ctx->npos * ctx->close;
  double total_value = assets_value + ctx->earnings - ctx->fees;
  /* Return on investment */
  double roi = ((total_value / ctx->amount) - 1.0) * 100.0;

  /* Basic informations */
  PR_STAT("amount: %.2lf, earnings: %.2lf, npos: %.2lf, fees: %.2lf\n",
	  ctx->amount, ctx->earnings, ctx->npos, ctx->fees);
  /* More stats */
  //PR_ERR("nbuy: %d, nsell: %d ", ctx->nbuy, ctx->nsell);
  /* Values */
  PR_STAT("assets_value: %.2lf, total_value: %.2lf, roi: %.2lf%%\n",
	  assets_value, total_value, roi);
  /* Interesting stuff */
  PR_STAT("balance: %.2lf, max_drawdown: %.2lf\n",
	  ctx->balance, ctx->max_drawdown);
}
