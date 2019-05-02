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

/* This is a initclass */

#define __timeline_entry__(x) ((struct timeline_entry*)(x))

#define __timeline_entry_init__(ctx, time, gr)                  \
  timeline_entry_init(__timeline_entry__(ctx), time, gr)
#define __timeline_entry_release__(ctx)                 \
  timeline_entry_release(__timeline_entry__(ctx))

#define __timeline_entry_timecmp__(ctx, time)           \
  timeline_entry_timecmp(__timeline_entry__(ctx), time)
#define __timeline_entry_find__(ctx, time)		\
  timeline_entry_find(__timeline_entry__(ctx), time)
#define __timeline_entry_str__(ctx)		\
  timeline_entry_str(__timeline_entry__(ctx))
#define __timeline_entry_str_r__(ctx, buf, len)                 \
  timeline_entry_str_r(__timeline_entry__(ctx), buf, len)

#define __timeline_entry_relative__(ctx, n)     \
  (typeof(ctx))__list_relative__((ctx), n)

struct timeline_entry {
  /* parent */
  __inherits_from__(struct list);
  
  /* Time/Date management */
  time_info_t time;
  time_gr_t gr;

  /* Indicators entries list */
  slist_head_t(struct indicator_entry) slist_ientry;
};

int timeline_entry_init(struct timeline_entry *ctx, time_info_t time, time_gr_t g);
void timeline_entry_release(struct timeline_entry *ctx);

/* Methods */
void timeline_entry_add_indicator_entry(struct timeline_entry *ctx, struct indicator_entry *ientry);
struct indicator_entry *timeline_entry_find_indicator_entry(struct timeline_entry *ctx, unique_id_t indicator_id);
time_info_t timeline_entry_timecmp(struct timeline_entry *ctx, time_info_t time);
struct timeline_entry *timeline_entry_find(struct timeline_entry *ctx, time_info_t time);

const char *timeline_entry_str(struct timeline_entry *ctx);
const char *timeline_entry_str_r(struct timeline_entry *ctx, char *buf, size_t len);

#endif
