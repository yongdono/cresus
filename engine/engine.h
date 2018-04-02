/*
 * Cresus EVO - engine.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/21/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef __Cresus_EVO__engine__
#define __Cresus_EVO__engine__

#include "engine/order.h"
#include "engine/position.h"
#include "engine/timeline.h"

struct engine {
  struct timeline *timeline;
  /* Order fifo */
  list_head_t(struct order) list_order;
  /* Position management. OBSOLETE */
  list_head_t(struct position) list_position_to_open;
  list_head_t(struct position) list_position_opened;
  list_head_t(struct position) list_position_to_close;
  list_head_t(struct position) list_position_closed;
  /* Money management */
  double npos;
  double amount; /* FIXME: find another name */
  double earnings; /* Find another name */
  double fees; /* Total fees */
  double balance; /* Buy/sell balance*/
  /* Statistics */
  int nbuy, nsell;
  double max_drawdown; /* Max money invested at the same time */
  double transaction_fee; /* Cost per transaction */
  /* Last close value (FIXME) */
  double close;
  /* Positions filter */
  time_info_t filter;
  /* Display info */
  int quiet;
};

#define engine_set_transaction_fee(ctx, fee)	\
  (ctx)->transaction_fee = fee
#define engine_set_filter(ctx, time_info)	\
  (ctx)->filter = time_info
#define engine_set_quiet(ctx, quiet)		\
  (ctx)->quiet = quiet

/* External pointer to plugin */
typedef int (*engine_feed_ptr)(struct engine*, struct timeline*, struct timeline_entry*);

int engine_init(struct engine *e, struct timeline *t);
void engine_release(struct engine *e);

void engine_run(struct engine *e, engine_feed_ptr feed);

int engine_place_order(struct engine *ctx, order_t type, order_by_t by,
		       double value);

void engine_display_stats(struct engine *ctx);

#endif /* defined(__Cresus_EVO__engine__) */
