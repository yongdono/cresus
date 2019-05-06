/*
 * Cresus EVO - indicator.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/21/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef INDICATOR_H
#define INDICATOR_H

#include "framework/types.h"
#include "framework/slist.h"
#include "framework/timeline.h"
#include "framework/slist_by_uid.h"

#define __indicator__(x) ((struct indicator*)(x))

/* Heritable */
#define __indicator_init__(ctx, id, feed, reset)        \
  indicator_init(__indicator__(ctx), id, feed, reset)
#define __indicator_release__(ctx)              \
  indicator_release(__indicator__(ctx))

/* Set methods */
#define __indicator_set_string__(ctx, fmt, ...)                         \
  snprintf(__indicator__(ctx)->str, INDICATOR_STR_MAX,                  \
	   fmt, ##__VA_ARGS__)
#define __indicator_set_event__(ctx, candle, event)		\
  indicator_set_event(__indicator__(ctx), candle, event)
#define __indicator_feed__(ctx, entry)          \
  indicator_feed(__indicator__(ctx), entry);

/* Internal values get */
#define __indicator_string__(ctx) __indicator__(ctx)->str
//#define __indicator_candle_value__(self) __indicator__(self)->value

/* Define types */
struct indicator; /* FIXME : find something more elegant */
typedef int (*indicator_feed_ptr)(struct indicator*, struct timeline_track_entry*);
typedef void (*indicator_reset_ptr)(struct indicator*);

struct indicator {
  /* Inherits from slist_by_uid */
  __inherits_from__(struct slist_by_uid);
  /* Fn pointers */
  indicator_feed_ptr feed;
  indicator_reset_ptr reset;
  /* Name */
#define INDICATOR_STR_MAX 64
  char str[INDICATOR_STR_MAX];
  /* Status */
  int is_empty;
};

int indicator_init(struct indicator *ctx, unique_id_t uid, indicator_feed_ptr feed, indicator_reset_ptr reset);
void indicator_release(struct indicator *ctx);

int indicator_feed(struct indicator *ctx, struct timeline_track_entry *e);
void indicator_reset(struct indicator *ctx);

/*
 * Indicator entry object
 */
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

#endif /* defined(INDICATOR_H) */
