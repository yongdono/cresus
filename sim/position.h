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

typedef enum {
  POSITION_LONG,
  POSITION_SHORT
} position_t;

struct position {
  /* slistable */
  __inherits_from_slist__;
  /* Some data */
  struct timeline *t;
  
  struct candle *in;
  struct candle *out;
  
  int n;
  position_t type;
};

int position_init(struct position *p, struct timeline *t,
		  position_t type, int n);
void position_release(struct position *p);

int position_in(struct position *p);
int position_out(struct position *p);

#endif
