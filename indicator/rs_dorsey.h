/*
 * Cresus EVO - rs_dorsey.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef RS_DORSEY_H
#define RS_DORSEY_H

/*
 * Dorsey Relative Strength
 * Very simple formula :
 * RSD = (close / close_index) * 100.0
 */

#include "framework/types.h"
#include "framework/alloc.h"
#include "framework/timeline.h"
#include "framework/indicator.h"

/* Entries */

struct rs_dorsey_entry {
  /* As below */
  __inherits_from__(struct indicator_entry);
  /* Single value */
  double value;
};

#define rs_dorsey_entry_alloc(ctx, parent, value)	\
  DEFINE_ALLOC(struct rs_dorsey_entry, ctx,		\
	       rs_dorsey_entry_init, parent, value)
#define rs_dorsey_entry_free(ctx)		\
  DEFINE_FREE(ctx, rs_dorsey_entry_release);

static inline int rs_dorsey_entry_init(struct rs_dorsey_entry *ctx,
				       struct indicator *parent,
				       double value)
{
  __indicator_entry_init__(ctx, parent);
  ctx->value = value;
  return 0;
}

static inline void rs_dorsey_entry_release(struct rs_dorsey_entry *ctx)
{
  __indicator_entry_release__(ctx);
}

/* Maion object */

#define rs_dorsey_alloc(ctx, uid, ref_track_uid)                        \
  DEFINE_ALLOC(struct rs_dorsey, ctx, rs_dorsey_init, uid, ref_track_uid)
#define rs_dorsey_free(ctx)			\
  DEFINE_FREE(ctx, rs_dorsey_release)

struct rs_dorsey {
  /* As always, inherits from indicator */
  __inherits_from__(struct indicator);
  /* Reference chart & internals */
  unique_id_t ref_track_uid;
  double ratio_prev;
};

int rs_dorsey_init(struct rs_dorsey *ctx, unique_id_t id, unique_id_t ref_track_uid);
void rs_dorsey_release(struct rs_dorsey *ctx);

#endif
