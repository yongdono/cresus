/*
 * Cresus EVO - timeline.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/06/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef TIMELINE_H
#define TIMELINE_H

#include "framework/list.h"
#include "framework/alloc.h"
#include "framework/slist.h"
#include "framework/time_info.h"

#define __timeline_append_entry__(ctx, entry)           \
  timeline_append_entry((struct timeline*)(ctx), entry)

#define TIMELINE_NAME_MAX 256

/* Object is allocatable */
#define timeline_alloc(t, name)                         \
  DEFINE_ALLOC(struct timeline, t, timeline_init, name)
#define timeline_free(t)			\
  DEFINE_FREE(t, timeline_release)

#include "framework/input.h"
#include "framework/indicator.h"
#include "framework/timeline_entry.h"

struct timeline {
  /* "slistable" */
  __inherits_from__(struct slist);
  /* Internals */
  char name[TIMELINE_NAME_MAX];
  /* Main data */
  list_head_t(struct timeline_entry) list_entry;
  slist_head_t(struct indicator) slist_indicator;
  /* Parser (?) */
  struct timeline_entry *current;
};

int timeline_init(struct timeline *ctx, const char *name);
void timeline_release(struct timeline *ctx);

void timeline_add_indicator(struct timeline *ctx, struct indicator *i);
void timeline_load(struct timeline *ctx, struct input *in);

/* TODO : obsolete ? */
struct timeline_entry *timeline_step(struct timeline *ctx);
int timeline_entry_current(struct timeline *ctx, struct timeline_entry **e);

#endif

