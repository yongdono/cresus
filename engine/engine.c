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
  /* Init portfolio */
  list_head_init(&ctx->list_position);
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
  list_head_release(&ctx->list_position);
}

static double engine_npos(struct engine *ctx)
{
  double ret = 0.0;
  struct position *p;
  
  __list_for_each__(&ctx->list_position, p)
    ret = ((p->type == BUY) ? (ret + p->n) : (ret - p->n));

  return ret;
}

static void engine_run_position_buy(struct engine *ctx,
				    struct position *p,
				    struct candle *c)
{
  /* Record open value */
  position_set_value(p, c->open);
  
  if(p->req == SHARES){
    double unit = position_unit_value(p);
    position_set_n(p, p->request.shares);
    PR_INFO("%s - Buy %.0lf securities at %.2lf (%.2lf) VALUE\n",
	    candle_str(c), p->n * 1.0, unit, p->n * unit);
    
    /* Some stats */
    ctx->amount += p->n * unit;
    ctx->balance -= p->n * unit;
    
  }else{
    position_set_n(p, p->request.cash / c->open);
    PR_INFO("%s - Buy %.4lf securities for %.2lf CASH\n",
	    candle_str(c), p->n * 1.0, p->request.cash);

    /* Some stats */
    ctx->amount += p->request.cash;
    ctx->balance -= p->request.cash;
  }
  
  /* Confirm position */
  position_confirm(p);
}

static void engine_run_position_sell(struct engine *ctx,
				     struct position *p,
				     struct candle *c)
{
  struct list *safe;
  struct position *lp;

  /* Stats */
  double req = 0.0;
  double cash = 0.0;
  double nsold = 0.0;
  
  if(req == SHARES) req = p->request.shares;
  else req = c->open / p->request.cash;
  
  __list_for_each_prev_safe__(&ctx->list_position, lp, safe){
    /* Parse positions backwards */
    int n = MIN(req, lp->n);

    /* No stopped or req positions */
    if(lp->status != POSITION_STATUS_CONFIRMED)
      continue;

    /* Output condition */
    if(req <= 0.0)
      break;

    /* Update counters */
    req -= n;
    lp->n -= n;
    nsold += n;
    cash += n * ((c->open - lp->cert.funding) / lp->cert.ratio);
  }

  /* Display info */
  if(p->req == SHARES){
    PR_WARN("%s - Sell %.0lf securities at %.2lf (%.2lf) VALUE\n",
	    candle_str(c), nsold, cash / nsold, cash);
  }else{
    PR_WARN("%s - Sell %.4lf securities for %.2lf CASH\n",
	    candle_str(c), nsold, cash);
  }
  
  /* The end */
  ctx->earnings += cash;
  ctx->balance += cash;
  __list_del__(p);
}

static void engine_run_position(struct engine *ctx,
				struct position *p,
				struct timeline_entry *e)
{
  struct candle *c = __timeline_entry_self__(e);

  switch(p->type){
  case BUY: engine_run_position_buy(ctx, p, c); break;
  case SELL: engine_run_position_sell(ctx, p, c); break;
  default: PR_WARN("Not implemented\n"); return;
  }
  
  ctx->fees += ctx->transaction_fee;
  ctx->max_drawdown = MIN(ctx->balance, ctx->max_drawdown);
}

void engine_run(struct engine *ctx, engine_feed_ptr feed)
{
  struct list *safe;
  struct position *p;
  struct timeline_entry *entry;
  
  while((entry = timeline_step(ctx->timeline)) != NULL){
    struct candle *c = __timeline_entry_self__(entry); /* FIXME ? */
    __list_for_each_safe__(&ctx->list_position, p, safe){
      /* First: check stoplosses */
      if(c->low <= p->cert.stoploss)
	p->status = POSITION_STATUS_STOPPED;

      /* 2nd : check if there are opening positions */
      if(p->status == POSITION_STATUS_REQUESTED)
	engine_run_position(ctx, p, entry); /* Run */

      /* 3rd: Remove useless positions */
      if(p->n <= 0.0)
	__list_del__(p);
    }
    
    /* Then : feed the engine */
    feed(ctx, ctx->timeline, entry);
    ctx->close = c->close;
  }
}

int engine_set_order(struct engine *ctx, position_t type,
		     double value, position_req_t req, 
		     struct cert *cert)
{
  struct position *p;
  struct timeline_entry *entry;
  
  if(timeline_entry_current(ctx->timeline, &entry) != -1){
    /* Filter orders if needed */
    if(TIMECMP(entry->time, ctx->filter, GRANULARITY_DAY) < 0)
      goto err;

    if(position_alloc(p, type, req, value, cert)){
      __list_add_tail__(&ctx->list_position, p);
      return 0;
    }
  }
  
 err:
  return -1;
}

int engine_place_order(struct engine *ctx, position_t type,
		       position_req_t by, double value)
{
  struct timeline_entry *entry;
  return engine_set_order(ctx, type, by, value, NULL);
}

void engine_display_stats(struct engine *ctx)
{
  struct position *p;
  
  double npos = 0.0;
  double assets_value = 0.0;
  
  __list_for_each__(&ctx->list_position, p){
    npos += p->n;
    assets_value += position_current_value(p, ctx->close);
  }
  
  /* What we got left */
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
