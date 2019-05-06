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
  /* Inherits from slist */
  __inherits_from__(struct slist);

#define INDICATOR_STR_MAX 64
  /* Fn pointers */
  indicator_feed_ptr feed;
  indicator_reset_ptr reset;
  /* Unique Id & name */
  unique_id_t uid;
  char str[INDICATOR_STR_MAX];
  /* Status */
  int is_empty;
};

int indicator_init(struct indicator *ctx, unique_id_t id, indicator_feed_ptr feed, indicator_reset_ptr reset);
void indicator_release(struct indicator *ctx);

int indicator_feed(struct indicator *ctx, struct timeline_track_entry *e);
void indicator_reset(struct indicator *ctx);

#endif /* defined(INDICATOR_H) */
