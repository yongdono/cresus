/*
 * Cresus EVO - position.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04.05.2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef POSITION_H
#define POSITION_H

#include "engine/candle.h"
#include "engine/timeline.h"
#include "framework/slist.h"

struct position {
  /* slistable */
  __inherits_from_slist__;
  /* Some data */
  struct timeline *t;
  struct candle *c;
  int n;
};

int position_init(struct position *p, struct timeline *t, int n);
void position_release(struct position *p);

/* FIXME */
int position_confirm(struct position *p);

#endif
