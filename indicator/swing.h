/*
 * Cresus EVO - swing.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef SWING_H
#define SWING_H

#include "framework/candle.h"
#include "framework/indicator.h"

#define SWING_MAX 3

typedef enum {
  SWING_NONE,
  SWING_TOP,
  SWING_BOTTOM
} swing_t;

struct swing {
  /* This is an indicator */
  __inherits_from_indicator__;
  
  int count;
  int index;

  swing_t type;
  struct candle pool[SWING_MAX];
};

int swing_init(struct swing *s, const struct candle *c);
void swing_free(struct swing *s);

int swing_feed(struct indicator *i, const struct candle *c);

/* Indicator-specific */
swing_t swing_type_get(struct swing *s);

#endif
