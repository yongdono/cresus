/*
 * Cresus EVO - linear_reg.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 17/03/2018
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef LINEAR_REG_H
#define LINEAR_REG_H

/*
 * Linear regression
 */

#include "math/average.h"
#include "framework/types.h"
#include "framework/alloc.h"
#include "framework/indicator.h"

/* Timeline entries object */

struct linear_reg_entry {
  /* As below */
  __inherits_from__(struct indicator_entry);
  /* Single value */
  double value; /* End value for us */
  /* debug */
  double a, b; /* As in Y = aX + b */
};

#define linear_reg_entry_alloc(ctx, parent, value)		\
  DEFINE_ALLOC(struct linear_reg_entry, ctx,			\
	       linear_reg_entry_init, parent, value)
#define linear_reg_entry_free(ctx)		\
  DEFINE_FREE(ctx, linear_reg_entry_release)

static inline int linear_reg_entry_init(struct linear_reg_entry *ctx,
					struct indicator *parent,
					double value)
{
  __indicator_entry_init__(ctx, parent);
  ctx->value = value;
  return 0;
}

static inline void linear_reg_entry_release(struct linear_reg_entry *ctx)
{
  __indicator_entry_release__(ctx);
}

/* Main object */

#define linear_reg_alloc(ctx, uid, period)                              \
  DEFINE_ALLOC(struct linear_reg, ctx, linear_reg_init, uid, period)
#define linear_reg_free(ctx)			\
  DEFINE_FREE(ctx, linear_reg_release)

struct linear_reg {
  /* As always */
  __inherits_from__(struct indicator);
  /* Internals */
  int period;
};

int linear_reg_init(struct linear_reg *ctx, unique_id_t id, int period);
void linear_reg_release(struct linear_reg *ctx);

#endif
