/*
 * Cresus EVO - timeline_entry.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef TIMELINE_ENTRY_H
#define TIMELINE_ENTRY_H

#include <time.h>
#include "list.h"

/* This is a superclass */

#define __inherits_from_timeline_entry__		\
  struct timeline_entry __parent_timeline_entry__
#define __timeline_entry__(x) (x)->__parent_timeline_entry__
#define __timeline_entry_self__(x) (x)->__self_timeline_entry__

#define __timeline_entry_super__(self, time)			\
  timeline_entry_init(&__timeline_entry__(self), self, time)
#define __timeline_entry_free__(self)			\
  timeline_entry_free(&__timeline_entry__(self))

#define __timeline_entry_set_time__(self, time)	\
  __timeline_entry__(self).time = time;
#define __timeline_entry_difftime__(self, time)			\
  timeline_entry_difftime(&__timeline_entry__(self), time)
#define __timeline_entry_localtime_str__(self, buf, len)	\
  timeline_entry_localtime_str(&__timeline__(self), buf, len)

struct timeline_entry {
  /* parent */
  __inherits_from_list__;
  void *__self_timeline_entry__;
  
  /* Time/Date management */
  time_t time; /* Epoch */
};

static inline int timeline_entry_init(struct timeline_entry *e,
				      void *self, time_t time) {
  __list_super__(e);
  e->__self_timeline_entry__ = self;
  e->time = time;
  return 0;
}

static inline void timeline_entry_free(struct timeline_entry *e) {
  __list_free__(e);
}

static inline time_t timeline_entry_difftime(struct timeline_entry *e,
					     time_t time) {
  return (e->time - time);
}

/* for debug purposes */
static inline const char *
timeline_entry_localtime_str(struct timeline_entry *e,
			     char *buf, size_t len)
{
  struct tm tm;
  time_t time = e->time;
  strftime(buf, len, "%c", gmtime_r(&time, &tm));
  return buf;
}

#endif
