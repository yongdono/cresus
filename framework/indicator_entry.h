/*
 * Cresus - indicator_entry.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 30/04/2019
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

/* Not allocatable directly */

#ifndef INDICATOR_ENTRY_H
#define INDICATOR_ENTRY_H

#include "framework/types.h"
#include "framework/slist.h"

#define __indicator_entry__(x) ((struct indicator_entry*)(x))

/* Heritable */
#define __indicator_entry_init__(ctx, iparent)                  \
  indicator_entry_init(__indicator_entry__(ctx), iparent)
#define __indicator_entry_release__(ctx)                \
  indicator_entry_release(__indicator_entry__(ctx))

struct indicator; /* Avoid circular dependency */

struct indicator_entry {
  __inherits_from__(struct slist);
  /* Remember who generated this entry */
  struct indicator *parent;
};

static inline int indicator_entry_init(struct indicator_entry *ctx,
				       struct indicator *parent)
{
  __slist_init__(ctx);
  ctx->parent = parent;
  return 0; /* alloc rulz */
}

static inline void indicator_entry_release(struct indicator_entry *ctx)
{
  __slist_release__(ctx);
}

#endif /* defined(INDICATOR_ENTRY_H) */
