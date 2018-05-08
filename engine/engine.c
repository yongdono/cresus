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
  /* Init orders fifo */
  list_head_init(&ctx->list_order);
  /* Money */
  ctx->npos_buy = 0;
  ctx->npos_sell = 0;
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
}

static void engine_run_order_buy(struct engine *ctx,
				 struct order *o,
				 struct candle *c)
{
  double npos;
  
  if(o->by == ORDER_BY_NB){
    npos = o->value;
    PR_INFO("%s - Buy %.0lf securities at %.2lf VALUE\n",
	    candle_str(c), o->value, c->open);
  }else{
    npos = (o->value / c->open);
    PR_INFO("%s - Buy %.4lf securities for %.2lf CASH\n",
	    candle_str(c), o->value / c->open, o->value);
  }
  
  /* Buy n positions */
  ctx->npos_buy += npos;
  ctx->amount += (npos * c->open);
  ctx->balance -= (npos * c->open);
  /* Stats */
  ctx->nbuy++;
}

static void engine_run_order_sell(struct engine *ctx,
				  struct order *o,
				  struct candle *c)
{
  double npos = engine_npos(ctx);
  
  /* Warning : no negative positions ! */
  if(o->by == ORDER_BY_NB){
    npos = MIN(npos, o->value);
    PR_WARN("%s - Sell %.0lf securities at %.2lf VALUE\n",
	    candle_str(c), npos, c->open);
  }else{
    npos = MIN(npos, (o->value / c->open));
    PR_WARN("%s - Sell %.4lf securities for %.2lf CASH\n",
	    candle_str(c), npos, npos * c->open);
  }

  if(npos > 0){
    /* Sell n positions */
    ctx->npos_sell += npos;
    ctx->earnings += (npos * c->open);
    ctx->balance += (npos * c->open);
    /* Stats */
    ctx->nsell++;
  }
}

static void engine_run_order_sell_all(struct engine *ctx,
				      struct order *o,
				      struct candle *c)
{
  double npos = engine_npos(ctx);
  
  PR_ERR("%s - Sell %.2lf securities for %.2lf TOTAL VALUE\n",
	 candle_str(c), npos, npos * c->open);
  
  ctx->earnings += (npos * c->open);
  ctx->balance += (npos * c->open);
  ctx->npos_sell += npos;
  /* Stats */
  ctx->nsell++;
}

static void engine_run_order(struct engine *ctx, struct order *o,
			     struct timeline_entry *e)
{
  struct candle *c = __timeline_entry_self__(e);

  switch(o->type){
  case ORDER_BUY: engine_run_order_buy(ctx, o, c); break;
  case ORDER_SELL: engine_run_order_sell(ctx, o, c); break;
  case ORDER_SELL_ALL: engine_run_order_sell_all(ctx, o, c); break;
  default: return;
  }
  
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
      /* Delete */
      __list_del__(order);
      order_free(order);
    }
    
    /* Then : feed the engine */
    feed(ctx, ctx->timeline, entry);
    ctx->close = c->close;
  }
}

int engine_set_order(struct engine *ctx, order_t type,
		     order_by_t by, double value,
		     struct cert *cert)
{
  struct order *order;
  struct timeline_entry *entry;
  
  if(timeline_entry_current(ctx->timeline, &entry) != -1){
    /* Filter orders if needed */
    if(TIMECMP(entry->time, ctx->filter, GRANULARITY_DAY) < 0)
      goto err;
    
    if(order_alloc(order, type, by, value, cert)){
      __list_add_tail__(&ctx->list_order, order);
      return 0;
    }
  }
  
 err:
  return -1;
}

int engine_place_order(struct engine *ctx, order_t type,
		       order_by_t by, double value)
{
  struct order *order;
  struct timeline_entry *entry;
  return engine_set_order(ctx, type, by, value, NULL);
}

void engine_display_stats(struct engine *ctx)
{
  /* What we got left */
  double npos = engine_npos(ctx);
  double assets_value = npos * ctx->close;
  double total_value = assets_value + ctx->earnings - ctx->fees;
  /* Return on investment */
  double roi = ((total_value / ctx->amount) - 1.0) * 100.0;

  /* Basic informations */
  PR_STAT("amount: %.2lf, earnings: %.2lf, npos: %.2lf, fees: %.2lf\n",
	  ctx->amount, ctx->earnings, npos, ctx->fees);
  /* More stats */
  //PR_ERR("nbuy: %d, nsell: %d ", ctx->nbuy, ctx->nsell);
  /* Values */
  PR_STAT("assets_value: %.2lf, total_value: %.2lf, roi: %.2lf%%\n",
	  assets_value, total_value, roi);
  /* Interesting stuff */
  PR_STAT("balance: %.2lf, max_drawdown: %.2lf\n",
	  ctx->balance, ctx->max_drawdown);
}
