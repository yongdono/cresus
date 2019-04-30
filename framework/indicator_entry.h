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

#include "framework/slist.h"

#define __inherits_from_indicator_entry__		\
  struct indicator_entry __parent_indicator_entry__
#define __indicator_entry_is_superclass__ void *__self_indicator_entry__
#define __indicator_entry__(x) (&(x)->__parent_indicator_entry__)
#define __indicator_entry_self__(x) ((x)->__self_indicator_entry__)
#define __indicator_entry_self_init__(x, self)	\
  (x)->__self_indicator_entry__ = self

#define __indicator_entry_super__(self, indicator)			\
  __indicator_entry_self_init__(__indicator_entry__(self), self);	\
  indicator_entry_init(__indicator_entry__(self), indicator);
#define __indicator_entry_release__(self)			\
  indicator_entry_release(__indicator_entry__(self));

struct indicator; /* Avoid circular dependency */

struct indicator_entry {
  __inherits_from_slist__;
  __indicator_entry_is_superclass__;
  /* Remember who generated this entry */
  struct indicator *parent;
};

static inline int indicator_entry_init(struct indicator_entry *ctx,
				       struct indicator *parent)
{
  __slist_super__(ctx);
  ctx->parent = parent;
  return 0; /* alloc rulz */
}

static inline void indicator_entry_release(struct indicator_entry *ctx)
{
  __slist_release__(ctx);
}

#endif /* defined(INDICATOR_ENTRY_H) */
