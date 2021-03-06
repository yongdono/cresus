/*
 * Cresus EVO - stoploss.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/17/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef STOPLOSS_H
#define STOPLOSS_H

#include "framework/types.h"
#include "framework/indicator.h"

typedef enum {
  STOPLOSS_UP,
  STOPLOSS_DOWN
} stoploss_t;

/* Events */
#define STOPLOSS_EVENT_HIT 0

struct stoploss {
  /* Inherits from indicator */
  __inherits_from__(struct indicator);
  
  /* Own data */
  stoploss_t type;
  double percent;
  double value;
  int trigger;
};

int stoploss_init(struct stoploss *s, unique_id_t id, double percent);
void stoploss_release(struct stoploss *s);

void stoploss_set(struct stoploss *s, stoploss_t type, double value);
void stoploss_clear(struct stoploss *s);

#endif
