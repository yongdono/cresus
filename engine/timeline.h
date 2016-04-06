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

struct timeline {
  struct timeline_entry list_entry;
  struct timeline_entry *cache;
  granularity_t g;
};

int timeline_init(struct timeline *t, granularity_t g, struct input *in);
void timeline_free(struct timeline *t);

/* TODO : Fill this */

#endif
