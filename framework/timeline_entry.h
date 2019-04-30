/*
 * Cresus EVO - timeline_entry.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef TIMELINE_ENTRY_H
#define TIMELINE_ENTRY_H

#include "framework/list.h"
#include "framework/types.h"
#include "framework/time_info.h"
#include "framework/indicator_entry.h"

/* This is a superclass */

#define __inherits_from_timeline_entry__		\
  struct timeline_entry __parent_timeline_entry__
#define __timeline_entry_is_superclass__ void *__self_timeline_entry__
#define __timeline_entry__(x) (&((x)->__parent_timeline_entry__))
#define __timeline_entry_self__(x) (x)->__self_timeline_entry__
#define __timeline_entry_self_init__(x, self)	\
  __timeline_entry_self__(x) = self

#define __timeline_entry_super__(self, time, granularity)		\
  __timeline_entry_self_init__(__timeline_entry__(self), self);		\
  timeline_entry_init(__timeline_entry__(self), time, granularity)
#define __timeline_entry_release__(self)                \
  timeline_entry_release(__timeline_entry__(self))

#define __timeline_entry_timecmp__(self, time)			\
  timeline_entry_timecmp(__timeline_entry__(self), time)
#define __timeline_entry_find__(self, time)		\
  timeline_entry_find(__timeline_entry__(self), time)
#define __timeline_entry_str__(self)		\
  timeline_entry_str(__timeline_entry__(self))
#define __timeline_entry_str_r__(self, buf, len)		\
  timeline_entry_str_r(__timeline_entry__(self), buf, len)

#define __timeline_entry_relative_self__(entry, n)		\
  __timeline_entry_self__((struct timeline_entry*)		\
			  __list_relative__((entry), n))

struct timeline_entry {
  /* parent */
  __inherits_from_list__;
  __timeline_entry_is_superclass__;
  
  /* Time/Date management */
  time_info_t time;
  granularity_t granularity;

  /* Indicators entries list */
  slist_head_t(struct indicator_entry) slist_ientry;
};

int timeline_entry_init(struct timeline_entry *ctx, time_info_t time, granularity_t g);
void timeline_entry_release(struct timeline_entry *ctx);

/* Methods */
void timeline_entry_add_indicator_entry(struct timeline_entry *ctx, struct indicator_entry *ientry);
struct indicator_entry *timeline_entry_find_indicator_entry(struct timeline_entry *ctx, unique_id_t indicator_id);
time_info_t timeline_entry_timecmp(struct timeline_entry *ctx, time_info_t time);
struct timeline_entry *timeline_entry_find(struct timeline_entry *ctx, time_info_t time);

const char *timeline_entry_str(struct timeline_entry *ctx);
const char *timeline_entry_str_r(struct timeline_entry *ctx, char *buf, size_t len);

#endif
