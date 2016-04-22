/*
 * Cresus EVO - timeline.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/06/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef TIMELINE_H
#define TIMELINE_H

#include "framework/alloc.h"
#include "framework/slist.h"
#include "framework/input.h"
#include "framework/indicator.h"
#include "framework/timeline_entry.h"

/* Automatically generated by script/superclass */
#define __inherits_from_timeline__ struct timeline __parent_timeline__
#define __timeline_is_superclass__ void *__self_timeline__
#define __timeline__(x) (&((x)->__parent_timeline__))
#define __timeline_self__(x) ((struct timeline*)x)->__self_timeline__
#define __timeline_self_init__(x, self) (x)->__self_timeline__ = self

#define __timeline_super__(self, name, input)		    \
  __timeline_self_init__(__timeline__(self), self);	    \
  timeline_init(__timeline__(self), name, input);
#define __timeline_free__(self) timeline_free(__timeline__(self));

#define TIMELINE_NAME_MAX 256

/* Object is allocatable */
#define timeline_alloc(t, name, input)				\
  DEFINE_ALLOC(struct timeline, t, timeline_init, name, input)

struct timeline {
  /* "Listable" & "Heritable" */
  __inherits_from_slist__;
  __timeline_is_superclass__;
  /* Data */
  struct input *in;
  char name[TIMELINE_NAME_MAX];
  /* Main data / graph */
  __list_head__(struct timeline_entry) list_entry;
  struct timeline_entry *current_timeline_entry;
  /* Secondary graphs */
  __slist_head__(struct indicator) slist_indicator;
};

int timeline_init(struct timeline *t, const char *name, struct input *in);
void timeline_free(struct timeline *t);

int timeline_add_indicator(struct timeline *t, struct indicator *i);
struct timeline_entry *timeline_next_entry(struct timeline *t);
struct timeline_entry *timeline_step(struct timeline *t);
int timeline_execute(struct timeline *t);

#endif
