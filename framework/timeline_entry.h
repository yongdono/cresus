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

/* Define furtherly required types */
typedef time_t granularity_t;

#define GRANULARITY_SEC   1
#define GRANULARITY_MIN   60
#define GRANULARITY_HOUR  3600
#define GRANULARITY_DAY   86400
#define GRANULARITY_WEEK  432000
#define GRANULARITY_MONTH 1728000
#define GRANULARITY_YEAR  20736000

/* This is a superclass */

#define __inherits_from_timeline_entry__		\
  struct timeline_entry __parent_timeline_entry__
#define __timeline_entry_is_superclass__ void *__self_timeline_entry__
#define __timeline_entry__(x) (x)->__parent_timeline_entry__
#define __timeline_entry_self__(x) (x)->__self_timeline_entry__

#define __timeline_entry_super__(self, time)			\
  timeline_entry_init(&__timeline_entry__(self), self, time)
#define __timeline_entry_free__(self)			\
  timeline_entry_free(&__timeline_entry__(self))

/* Not required at the moment
#define __timeline_entry_set_time__(self, time)	\
  __timeline_entry__(self).time = time;
*/
#define __timeline_entry_timecmp__(self, time, granularity)		\
  timeline_entry_timecmp(&__timeline_entry__(self), time, granularity)
#define __timeline_entry_localtime_str__(self, buf, len)	\
  timeline_entry_localtime_str(&__timeline__(self), buf, len)

struct timeline_entry {
  /* parent */
  __inherits_from_list__;
  __timeline_entry_is_superclass__;
  
  /* Time/Date management */
  time_t time; /* Epoch */
};

static inline int timeline_entry_init(struct timeline_entry *e,
				      void *self, time_t time) {
  __list_super__(e);
  __timeline_entry_self__(e) = self;

  e->time = time;
  return 0;
}

static inline void timeline_entry_free(struct timeline_entry *e) {
  __list_free__(e);
}

#define __timeline_entry_time(time, granularity)	\
  (time - (time % granularity))

static inline time_t timeline_entry_timecmp(struct timeline_entry *e,
					    time_t time, granularity_t g) {
  return (__timeline_entry_time(e->time, g) -
	  __timeline_entry_time(time, g));
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
