/*
 * Cresus EVO - rs_roc.h
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 15/10/2018
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef RS_ROC_H
#define RS_ROC_H

/*
 * Relative Strength / ROC comparison
 */

#include "framework/list.h"
#include "framework/types.h"
#include "framework/alloc.h"
#include "framework/indicator.h"

struct rs_roc_entry {
  /* As always... */
  __inherits_from__(struct indicator_entry);
  /* Data */
  double value;
  double roc, roc_ref;
};

#define rs_roc_entry_alloc(ctx, parent, value, roc, roc_ref)		\
  DEFINE_ALLOC(struct rs_roc_entry, ctx, rs_roc_entry_init, parent,	\
	       value, roc, roc_ref)
#define rs_roc_entry_free(ctx)                  \
  DEFINE_FREE(ctx, rs_roc_entry_free)

static inline int rs_roc_entry_init(struct rs_roc_entry *ctx,
				    struct indicator *parent,
				    double value, double roc,
                                    double roc_ref)
{
  __indicator_entry_init__(ctx, parent);
  ctx->value = value;
  ctx->roc = roc;
  ctx->roc_ref = roc_ref;
  return 0;
}

static inline void rs_roc_entry_release(struct rs_roc_entry *ctx)
{
  __indicator_entry_release__(ctx);
}

/* Main object */

#define rs_roc_alloc(ctx, uid, period, ref_track_uid)                   \
  DEFINE_ALLOC(struct rs_roc, ctx, rs_roc_init, uid, period, ref_track_uid)
#define rs_roc_free(ctx)			\
  DEFINE_FREE(ctx, rs_roc_release)

struct rs_roc {
  /* Inherits from indicator */
  __inherits_from__(struct indicator);
  /* Internals */
  int period;
  unique_id_t ref_track_uid;
};

int rs_roc_init(struct rs_roc *ctx, unique_id_t id, int period, unique_id_t ref_track_uid);
void rs_roc_release(struct rs_roc *ctx);

#endif
