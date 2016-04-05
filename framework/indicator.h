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

#include "engine/candle.h"

#define INDICATOR_STR_MAX 64

struct indicator_timeline_entry {
  __inherits_from_timeline_entry__;
  /* Basic info */
  unsigned int status;
  void *data;
};

/* As it's a superclass, we want macros to manipulate this */
#define __inherits_from_indicator__ struct indicator __parent_indicator__
#define __indicator__(x) (x)->__parent_indicator__
#define __indicator_self__(x) (x)->__self_indicator__

#define __indicator_super__(self, value, feed)			\
  indicator_init(&__indicator__(self), self, value, feed)
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
struct indicator; /* FIXME ; find something more elegant */
typedef int (*indicator_feed_ptr)(struct indicator*, struct timeline_entry*);

struct indicator {
  /* Inherits from slist */
  __inherits_from_slist__;
  /* For easier manipulation */
  void *__self_indicator__;
  
  candle_value_t value;
  indicator_feed_ptr feed;
  char str[INDICATOR_STR_MAX];
};

int indicator_init(struct indicator *i,    /* Required */
		   void *self,             /* Required */
		   candle_value_t value,   /* Dunno what to do with this */
		   indicator_feed_ptr feed /* Can do better */
		   );

void indicator_free(struct indicator *i);

int indicator_feed(struct indicator *i, const struct candle *candle);
void indicator_set_event(struct indicator *i, struct candle *candle, int event);

#endif /* defined(__Cresus_EVO__indicator__) */
