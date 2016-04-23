/*
 * Cresus EVO - macd.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef MACD_H
#define MACD_H

#include <stdlib.h>

#include "math/average.h"
#include "framework/alloc.h"
#include "framework/indicator.h"

struct macd_indicator_entry {
  /* As always */
  __inherits_from_indicator_entry__;
  /* Own data */
  double value;
  double signal;
  double histogram;
};

#define macd_indicator_entry_alloc(entry, parent, value, signal)	\
  DEFINE_ALLOC(struct macd_indicator_entry, entry,			\
	       macd_indicator_entry_init, parent, value, signal)

static inline int
macd_indicator_entry_init(struct macd_indicator_entry *entry,
			  struct indicator *parent,
			  double value, double signal){
  __indicator_entry_super__(entry, parent);
  entry->value = value;
  entry->signal = signal;
  entry->histogram = (value - signal);
  return 0;
}

/* Indicator events */
#define MACD_EVENT_SIGNAL0 0 /* TODO */
#define MACD_EVENT_SIGNAL1 0 /* TODO */
#define MACD_EVENT_SIGNAL2 0 /* TODO */
#define MACD_EVENT_SIGNAL3 0 /* TODO */

/* Default params */
#define MACD_DEFAULT_FAST_P   12
#define MACD_DEFAULT_SLOW_P   26
#define MACD_DEFAULT_SIGNAL_P 9

#define macd_alloc(m, id, fast_p, slow_p, signal_p)			\
  DEFINE_ALLOC(struct macd, m, macd_init, id, fast_p, slow_p, signal_p)

struct macd {
  /* Parent */
  __inherits_from_indicator__;
  /* 3 averages required */
  struct average fast;
  struct average slow;
  struct average signal;
};

int macd_init(struct macd *m, indicator_id_t id,
	      int fast_p, int slow_p, int signal_p);
void macd_free(struct macd *m);

#endif
