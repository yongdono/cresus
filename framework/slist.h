/*
 * Cresus EVO - slist.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef SLIST_H
#define SLIST_H

#include <stdio.h>
#include "framework/types.h"

#define __slist__(x) ((struct slist*)(x))

/* Exports */
#define __slist_init__(ctx) slist_init(__slist__(ctx))
#define __slist_release__(ctx) slist_release(__slist__(ctx))
#define __slist_insert__(ctx, n3)                    \
  slist_insert(__slist__(ctx), __slist__(n3))
#define __slist_del__(ctx) slist_del(__slist__(ctx))
/* Iteration */
#define __slist_for_each__(head, ctx)                           \
  for(struct slist *__ptr__ = __slist__(head)->next;            \
      __ptr__ != NULL && (ctx = (typeof(ctx))(__ptr__));        \
      __ptr__ = __ptr__->next)
  
/* Basic slist object */

#define slist_head_t(type) struct slist /* Type is indicative */
#define slist_head_init(x) slist_init(x)
#define slist_head_release(x) slist_release(x)

struct slist {
  struct slist *next;
};

static inline int slist_init(struct slist *ctx)
{
  ctx->next = NULL;
  return 0;
}

static inline void slist_release(struct slist *ctx)
{
  ctx->next = NULL;
}

static inline void slist_insert(struct slist *ctx, struct slist *n3)
{
  n3->next = ctx->next;
  ctx->next = n3;
}

static inline void slist_del(struct slist *ctx)
{
  ctx->next = ctx->next->next;
}

#define slist_is_last(ctx) ((ctx)->next == NULL)
#define slist_is_empty(head) slist_is_last(head)
#define slist_for_each(head, ptr)				\
  for(ptr = (head)->next; (ptr) != NULL; ptr = (ptr)->next)

#endif
