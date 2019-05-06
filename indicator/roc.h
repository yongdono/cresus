/*
 * Cresus EVO - roc.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 02.05.2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef ROC_H
#define ROC_H

/*
 * Rate of Change
 * Momentum indicator
 * Typical period is 12
 */

#include "math/average.h"

#include "framework/types.h"
#include "framework/alloc.h"
#include "framework/indicator.h"

struct roc_entry {
  /* As always... */
  __inherits_from__(struct indicator_entry);
  /* Data */
  double value;
};

#define roc_entry_alloc(entry, parent, value)				\
  DEFINE_ALLOC(struct roc_entry, entry, roc_entry_init, parent, value)
#define roc_entry_free(entry)			\
  DEFINE_FREE(entry, roc_entry_free)

static inline int roc_entry_init(struct roc_entry *ctx,
				 struct indicator *parent,
				 double value)
{
  __indicator_entry_init__(ctx, parent);
  ctx->value = value;
  return 0;
}

static inline void roc_entry_release(struct roc_entry *ctx)
{
  __indicator_entry_release__(ctx);
}

/* Main object */

#define roc_alloc(ctx, uid, period, average)			\
  DEFINE_ALLOC(struct roc, ctx, roc_init, uid, period, average)
#define roc_free(ctx)				\
  DEFINE_FREE(ctx, roc_release)

struct roc {
  /* Inherits from indicator */
  __inherits_from__(struct indicator);
  
  int period;
  struct average average;
};

int roc_init(struct roc *ctx, unique_id_t uid, int period, int average);
void roc_release(struct roc *ctx);

int roc_compute(struct roc *ctx, struct timeline_track_entry *e, double *rvalue);
void roc_reset(struct roc *ctx);

#endif
