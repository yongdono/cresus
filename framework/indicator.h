/*
 * Cresus EVO - indicator.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/21/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef __Cresus_EVO__indicator__
#define __Cresus_EVO__indicator__

#include "candle.h"

/* As it's a superclass, we want macros to manipulate this */
#define __inherits_from_indicator__ struct indicator __parent_indicator__
#define __indicator__(x) (x)->__parent_indicator__
#define __indicator_self__(x) (x)->__self_indicator__

#define __indicator_super__(self, value, feed)			\
  indicator_init(&__indicator__(self), self, value, feed)
#define __indicator_free__(self)		\
  indicator_free(&__indicator__(self))

#define __indicator_set_event__(self, candle, event)		\
  indicator_set_event(&__indicator__(self), candle, event)
#define __indicator_candle_value__(self) __indicator__(self).value

/* Define types */
struct indicator;
typedef int (*indicator_feed_ptr)(struct indicator*, const struct candle*);

struct indicator {
  candle_value_t value;
  indicator_feed_ptr feed;
  /* For easier manipulation */
  void *__self_indicator__;
};

int indicator_init(struct indicator *i, void *self, candle_value_t value, indicator_feed_ptr feed);
void indicator_free(struct indicator *i);

int indicator_feed(struct indicator *i, const struct candle *candle);
void indicator_set_event(struct indicator *i, struct candle *candle, int event);

#endif /* defined(__Cresus_EVO__indicator__) */
