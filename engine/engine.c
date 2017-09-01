/*
 * Cresus EVO - engine.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 09/01/2017
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#include <stdio.h>
#include "engine.h"

int engine_init(struct engine *ctx, struct timeline *t) {

  ctx->timeline = t;
  /* Init slists */
  list_head_init(&ctx->list_position_to_open);
  list_head_init(&ctx->list_position_opened);
  list_head_init(&ctx->list_position_to_close);
  list_head_init(&ctx->list_position_closed);
  
  /* Stat */
  ctx->factor = 1.0;
  ctx->nwin = 0;
  ctx->nloss = 0;
  
  return 0;
}

void engine_release(struct engine *ctx) {
  /* Nothing to do */
  list_head_release(&ctx->list_position_to_open);
  list_head_release(&ctx->list_position_opened);
  list_head_release(&ctx->list_position_to_close);
  list_head_release(&ctx->list_position_closed);
}

static void engine_xfer_positions(struct engine *ctx,
				  list_head_t(struct position) *dst,
				  list_head_t(struct position) *src,
				  position_action_t position_action) {
  
  struct list *safe;
  struct position *p;
  
  __list_for_each_safe__(src, p, safe){
    __list_del__(p);
    /* Insert in dst */
    __list_add_tail__(dst, p);
    /* Act on position */
    position_action(p);
  }
}

void engine_run(struct engine *ctx, engine_feed_ptr feed) {

  struct timeline_entry *entry;
  while((entry = timeline_step(ctx->timeline)) != NULL){
    /* First : check if there are opening positions */
    engine_xfer_positions(ctx, &ctx->list_position_opened,
			  &ctx->list_position_to_open,
			  position_in);
    /* Second : check if there are closing positions */
    engine_xfer_positions(ctx, &ctx->list_position_closed,
			  &ctx->list_position_to_close,
			  position_out);
    /* Third  : feed the engine */
    feed(ctx, ctx->timeline, entry);
  }
}

int engine_open_position(struct engine *ctx, struct timeline *t, 
			 position_t type, double n) {
  
  struct position *p;
  if(position_alloc(p, t, type, n)){
    __list_add_tail__(&ctx->list_position_to_open, p);
    return 0;
  }

  return -1;
}

int engine_close_position(struct engine *ctx, struct timeline *t) {
  
  int n = 0;
  struct list *safe;
  struct position *p;
  
  __list_for_each_safe__(&ctx->list_position_opened, p, safe){
    if(p->t == t){ /* FIXME : use timeline's name ? */
      __list_del__(p);
      __list_add_tail__(&ctx->list_position_to_close, p);
      n++;
    }
  }
  
  return n;
}

int engine_close_all_positions(struct engine *ctx) {
  
  engine_xfer_positions(ctx, &ctx->list_position_to_close,
			&ctx->list_position_opened,
			position_nop);
  
  return 0;
}
