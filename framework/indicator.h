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
#include "framework/timeline_entry.h"

/* As it's a superclass, we want macros to manipulate this */
#define __inherits_from_indicator__ struct indicator __parent_indicator__
#define __indicator_is_superclass__ void *__self_indicator__
#define __indicator__(x) (&(x)->__parent_indicator__)
#define __indicator_self__(x) (x)->__self_indicator__
#define __indicator_self_init__(x, self) (x)->__self_indicator__ = self

#define __indicator_super__(self, id, feed, reset)	\
  __indicator_self_init__(__indicator__(self), self);	\
  indicator_init(__indicator__(self), id, feed, reset)
#define __indicator_release__(self)		\
  indicator_release(__indicator__(self))

/* Set methods */
#define __indicator_set_string__(self, fmt, ...)			\
  snprintf(__indicator__(self)->str, INDICATOR_STR_MAX,			\
	   fmt, ##__VA_ARGS__)
#define __indicator_set_event__(self, candle, event)		\
  indicator_set_event(__indicator__(self), candle, event)
#define __indicator_feed__(self, entry)		\
  indicator_feed(__indicator__(self), entry);

/* Internal values get */
#define __indicator_string__(self) __indicator__(self)->str
//#define __indicator_candle_value__(self) __indicator__(self)->value

/* Define types */
struct indicator; /* FIXME : find something more elegant */
typedef int (*indicator_feed_ptr)(struct indicator*, struct timeline_entry*);
typedef void (*indicator_reset_ptr)(struct indicator*);

struct indicator {
  /* Inherits from slist */
  __inherits_from_slist__;
  __indicator_is_superclass__;

#define INDICATOR_STR_MAX 64
  /* Fn pointers */
  indicator_feed_ptr feed;
  indicator_reset_ptr reset;
  /* Unique Id & name */
  unique_id_t id;
  char str[INDICATOR_STR_MAX];
  /* Status */
  int is_empty;
};

int indicator_init(struct indicator *ctx, unique_id_t id, indicator_feed_ptr feed, indicator_reset_ptr reset);
void indicator_release(struct indicator *ctx);

int indicator_feed(struct indicator *ctx, struct timeline_entry *e);
void indicator_reset(struct indicator *ctx);

#endif /* defined(INDICATOR_H) */
