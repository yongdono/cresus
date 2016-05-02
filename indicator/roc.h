/*
 * Cresus EVO - roc.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 02.05.2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef ROC_H
#define ROC_H

/*
 * Rate of Change
 * Momentum indicator
 * Typical period is 12
 */

#include "math/average.h"
#include "engine/candle.h"
#include "framework/alloc.h"
#include "framework/indicator.h"

struct roc_entry {
  /* As always... */
  __inherits_from_indicator_entry__;
  /* Data */
  double value;
  double average;
};

#define roc_entry_alloc(entry, parent, value, average)	\
  DEFINE_ALLOC(struct roc_entry, entry, roc_entry_init, \
	       parent, value, average)
#define roc_entry_free(entry)			\
  DEFINE_FREE(entry, roc_entry_free)

static inline int roc_entry_init(struct roc_entry *entry,
				 struct indicator *parent,
				 double value, double average) {
  __indicator_entry_super__(entry, parent);
  entry->value = value;
  entry->average = average;
  return 0;
}

static inline void roc_entry_release(struct roc_entry *entry) {
  __indicator_entry_release__(entry);
}

/* Main object */

#define roc_alloc(roc, id, period, average)	\
  DEFINE_ALLOC(struct roc, roc, roc_init, id, period, average)
#define roc_free(roc)				\
  DEFINE_FREE(roc, roc_release)

struct roc {
  /* Inherits from indicator */
  __inherits_from_indicator__;
  
  struct average average;
  struct candle *last;
};

int roc_init(struct roc *r, indicator_id_t id, int period, int average);
void roc_release(struct roc *r);

#endif