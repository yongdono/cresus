/*
 * Cresus EVO - timeline.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/06/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef TIMELINE_H
#define TIMELINE_H

#include "framework/input.h"
#include "framework/timeline_entry.h"

#define TIMELINE_NAME_MAX 256

struct timeline {
  char name[TIMELINE_NAME_MAX];
  struct timeline_entry list_entry;
  struct timeline_entry *cache;
};

int timeline_init(struct timeline *t, struct input *in);
void timeline_free(struct timeline *t);

int timeline_load(struct timeline *t, struct input *in);

#endif
