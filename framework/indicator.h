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

/* Required to force data types */
#include "engine/candle.h" /* FIXME : Remove this ? */

#define __inherits_from_indicator_timeline_entry__			\
  struct indicator_timeline_entry __parent_indicator_timeline_entry__
#define __indicator_timeline_entry_is_superclass__	\
  void *__self_indicator_timeline_entry__
#define __indicator_timeline_entry__(x)		\
  (x)->__parent_indicator_timeline_entry__
#define __indicator_timeline_entry_self__(x)	\
  (x)->__self_indicator_timeline_entry__

#define __indicator_timeline_entry_super__(self)			\
  indicator_timeline_entry_init(&__indicator_timeline_entry__(self), self);
#define __indicator_timeline_entry_free__(self)				\
  indicator_timeline_entry_free(&__indicator_timeline_entry__(self));

struct indicator_timeline_entry {
  __inherits_from_timeline_entry__;
  __indicator_timeline_entry_is_superclass__;
  /* Basic info */
  unsigned int status; /* Maybe */
};

int indicator_timeline_entry_init(struct indicator_timeline_entry *entry, void *self);
void indicator_timeline_entry_free(struct indicator_timeline_entry *entry);

/* As it's a superclass, we want macros to manipulate this */
#define __inherits_from_indicator__ struct indicator __parent_indicator__
#define __indicator_is_superclass__ void *__self_indicator__
#define __indicator__(x) (x)->__parent_indicator__
#define __indicator_self__(x) (x)->__self_indicator__

#define __indicator_super__(self, feed)			\
  indicator_init(&__indicator__(self), self, feed)
#define __indicator_free__(self)		\
  indicator_free(&__indicator__(self))

/* Set methods */
#define __indicator_set_string__(self, fmt, ...)			\
  snprintf(__indicator__(self).str, INDICATOR_STR_MAX,			\
	   fmt, ##__VA_ARGS__)
#define __indicator_set_event__(self, candle, event)		\
  indicator_set_event(&__indicator__(self), candle, event)
#define __indicator_feed__(self, entry)				\
  __indicator__(self)->feed(&__indicator__(self), entry)

/* Internal values get */
#define __indicator_string__(self) __indicator__(self).str
#define __indicator_candle_value__(self) __indicator__(self).value

/* Define types */
struct indicator; /* FIXME : find something more elegant */
typedef int (*indicator_feed_ptr)(struct indicator*, const struct candle*);

struct indicator {
  /* Inherits from slist */
  __inherits_from_slist__;
  __indicator_is_superclass__;

#define INDICATOR_STR_MAX 64
  candle_value_t value;
  indicator_feed_ptr feed;
  char str[INDICATOR_STR_MAX];
};

int indicator_init(struct indicator *i, void *self, /* Required */
		   indicator_feed_ptr feed);        /* TODO : can do better */
void indicator_free(struct indicator *i);

/* TODO : fix this */
void indicator_set_event(struct indicator *i, struct candle *candle, int event);

#endif /* defined(__Cresus_EVO__indicator__) */
