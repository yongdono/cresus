/*
 * Cresus EVO - timeline.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef TIMELINE_H
#define TIMELINE_H

#include "list.h"
#include "input.h"
#include "candle.h"

struct timeline {
  struct list list_candle;
};

int timeline_init(struct timeline *t, struct input *in);
void timeline_free(struct timeline *t);

#endif
