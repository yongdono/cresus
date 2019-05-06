/*
 * Cresus EVO - lowest.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 08.05.2018
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef LOWEST_H
#define LOWEST_H

/*
 * Lowest
 * Some support indicator
 */

#include "framework/types.h"
#include "framework/alloc.h"
#include "framework/indicator.h"

struct lowest_entry {
  /* As always... */
  __inherits_from__(struct indicator_entry);
  /* Data */
  double value;
};

#define lowest_entry_alloc(ctx, parent)                                 \
  DEFINE_ALLOC(struct lowest_entry, ctx, lowest_entry_init, parent)
#define lowest_entry_free(ctx)                  \
  DEFINE_FREE(ctx, lowest_entry_free)

static inline int lowest_entry_init(struct lowest_entry *ctx,
                                    struct indicator *parent)
{
  __indicator_entry_init__(ctx, parent);
  return 0;
}

static inline void lowest_entry_release(struct lowest_entry *ctx)
{
  __indicator_entry_release__(ctx);
}

/* Main object */

#define lowest_alloc(ctx, uid, period)				\
  DEFINE_ALLOC(struct lowest, ctx, lowest_init, uid, period)
#define lowest_free(ctx)			\
  DEFINE_FREE(ctx, lowest_release)

struct lowest {
  /* Inherits from indicator */
  __inherits_from__(struct indicator);
  int period;
};

int lowest_init(struct lowest *ctx, unique_id_t uid, int period);
void lowest_release(struct lowest *ctx);

void lowest_reset(struct lowest *ctx);

#endif
