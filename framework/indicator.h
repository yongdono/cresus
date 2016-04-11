/*
 * Cresus EVO - indicator.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/21/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef __Cresus_EVO__indicator__
#define __Cresus_EVO__indicator__

#include "slist.h"
#include "timeline_entry.h"

/* Required to force data types 
 * FIXME ? */
#include "engine/candle.h"

/* TODO : find something for indicator data storage */

#define __inherits_from_indicator_entry__		\
  struct indicator_entry __parent_indicator_entry__
#define __indicator_entry_is_superclass__ void *__self_indicator_entry__
#define __indicator_entry__(x) (&(x)->__parent_indicator_entry__)
#define __indicator_entry_self__(x) ((x)->__self_indicator_entry__)
#define __indicator_entry_self_init__(x, self)	\
  (x)->__self_indicator_entry__ = self

#define __indicator_entry_super__(self)					\
  __indicator_entry_self_init__(__indicator_entry__(self), self);	\
  indicator_entry_init(__indicator_entry__(self));
#define __indicator_entry_free__(self)			\
  indicator_entry_free(__indicator_entry__(self));

struct indicator_entry {
  __inherits_from_slist__;
  __indicator_entry_is_superclass__;
  /* Say who is parent ? */
};

static inline int indicator_entry_init(struct indicator_entry *entry) {
  __slist_super__(entry);
}

static inline void indicator_entry_free(struct indicator_entry *entry) {
  __slist_free__(entry);
}

/* As it's a superclass, we want macros to manipulate this */
#define __inherits_from_indicator__ struct indicator __parent_indicator__
#define __indicator_is_superclass__ void *__self_indicator__
#define __indicator__(x) (&(x)->__parent_indicator__)
#define __indicator_self__(x) (x)->__self_indicator__
#define __indicator_self_init__(x, self) (x)->__self_indicator__ = self

#define __indicator_super__(self, feed)			\
  __indicator_self_init__(__indicator__(self), self);	\
  indicator_init(__indicator__(self), feed)
#define __indicator_free__(self)		\
  indicator_free(__indicator__(self))

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
#define __indicator_candle_value__(self) __indicator__(self)->value

/* Define types */
struct indicator; /* FIXME : find something more elegant */
typedef int (*indicator_feed_ptr)(struct indicator*, struct candle*);
/* TODO : why use ony candle data (seed / current) as we could set ref candle
 * in the init and then increment by granularity using some kind of step()
 * function ?
 */

struct indicator {
  /* Inherits from slist */
  __inherits_from_slist__;
  __indicator_is_superclass__;

#define INDICATOR_STR_MAX 64
  indicator_feed_ptr feed;
  /* Unique Id & name */
  unsigned int id;
  char str[INDICATOR_STR_MAX];
  /* Status */
  int is_empty;
  /* Data / graph */
  __list_head__(struct timeline_entry) list_entry;
};

int indicator_init(struct indicator *i, indicator_feed_ptr feed);
void indicator_free(struct indicator *i);

int indicator_feed(struct indicator *i, struct candle *c);
/* TODO : fix this */
void indicator_set_event(struct indicator *i, struct candle *candle, int event);

#endif /* defined(__Cresus_EVO__indicator__) */
