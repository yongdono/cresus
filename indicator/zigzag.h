/*
 * Cresus EVO - zigzag.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef ZIGZAG_H
#define ZIGZAG_H

#include "framework/types.h"
#include "framework/alloc.h"
#include "framework/indicator.h"

/* Common */

typedef enum {
  ZIGZAG_NONE,
  ZIGZAG_UP,
  ZIGZAG_DOWN,
} zigzag_dir_t;

/* Entries */

#define zigzag_entry_alloc(ctx, parent, dir, value, count)	\
  DEFINE_ALLOC(struct zigzag_entry, ctx,			\
	       zigzag_entry_init, parent, dir, value, count)
#define zigzag_entry_free(ctx)                  \
  DEFINE_FREE(ctx, zigzag_entry_release)

struct zigzag_entry {
  /* As always */
  __inherits_from__(struct indicator_entry);
  /* Self */
  double value;
  zigzag_dir_t dir;
  int n_since_last_ref;
  struct timeline_track_entry *ref;
};

static inline int zigzag_entry_init(struct zigzag_entry *ctx,
				    struct indicator *parent,
				    zigzag_dir_t dir, double value,
				    int count)
{
  __indicator_entry_init__(ctx, parent);
  ctx->dir = dir;
  ctx->value = value;
  ctx->n_since_last_ref = count;
  return 0;
}

static inline void zigzag_entry_release(struct zigzag_entry *ctx)
{
  __indicator_entry_release__(ctx);
}

/* Indicator */

#define zigzag_alloc(ctx, uid, thres, value)				\
  DEFINE_ALLOC(struct zigzag, ctx, zigzag_init, uid, thres, value)
#define zigzag_free(ctx)                        \
  DEFINE_FREE(ctx, zigzag_release)

struct zigzag {
  /* As usual */
  __inherits_from__(struct indicator);
  /* Params */
  double threshold;
  input_entry_value_t value;
  /* Internals */
  zigzag_dir_t dir;
  /* Some other stuff */
  int ref_count; /* Candles since base ref */
  struct timeline_track_entry *ref, *base_ref;
};

int zigzag_init(struct zigzag *ctx, unique_id_t uid, double threshold, input_entry_value_t value);
void zigzag_release(struct zigzag *ctx);

#endif
