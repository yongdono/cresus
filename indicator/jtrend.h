/*
 * Cresus EVO - jtrend.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 02/05/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef JTREND_H
#define JTREND_H

#include "math/average.h"
#include "framework/types.h"
#include "framework/alloc.h"
#include "framework/indicator.h"
/* Sub-indicators */
#include "indicator/roc.h"

struct jtrend_entry {
  /* Always */
  __inherits_from__(struct indicator_entry);
  /* Value type ? */
  double value;
  double ref_value;
};

#define jtrend_entry_alloc(entry, parent, value, ref_value)		\
  DEFINE_ALLOC(struct jtrend_entry, entry, jtrend_entry_init,		\
	       parent, value, ref_value)
#define jtrend_entry_free(entry)		\
  DEFINE_FREE(entry, jtrend_entry_release)

static inline int jtrend_entry_init(struct jtrend_entry *entry,
				    struct indicator *parent,
				    double value, double ref_value)
{
  __indicator_entry_init__(entry, parent);
  entry->value = value;
  entry->ref_value = ref_value;
  return 0;
}

static inline void jtrend_entry_release(struct jtrend_entry *entry)
{
  __indicator_entry_release__(entry);
}

/* Main object */

#define jtrend_alloc(jt, uid, period, average)                           \
  DEFINE_ALLOC(struct jtrend, jt, jtrend_init, uid, period, average)
#define jtrend_free(jt)				\
  DEFINE_FREE(jt, jtrend_release)

struct jtrend {
  /* Anyway */
  __inherits_from__(struct indicator);
  /* Some other data */
  struct roc roc;
  struct roc roc_ref;
  /* Referecne flow */
  unique_id_t ref_track_uid;
};

int jtrend_init(struct jtrend *ctx, unique_id_t uid, int period, int average, unique_id_t ref_track_uid);
void jtrend_release(struct jtrend *ctx);

#endif
