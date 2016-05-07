/*
 * Cresus EVO - jtrend.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 02/05/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "jtrend.h"

static int jtrend_feed(struct indicator *i,
		       struct timeline_entry *e) {

  struct jtrend_entry *jentry;
  struct timeline_entry *entry;

  struct jtrend *j = __indicator_self__(i);
  struct candle *c = __timeline_entry_self__(e);

  if((entry = timeline_entry_find(__list_self__(j->ref), e->time))){
    double value, average;
    roc_compute(&j->roc, e, &value, &average);
    roc_compute(&j->roc_ref, __timeline_entry_self__(entry),
		&value, &average);

    /* TODO : Alloc jentry & store in candle */

    j->ref = __list__(entry);
    return 1;
  }

  /* Do something here */
  return 0;
}

int jtrend_init(struct jtrend *j, indicator_id_t id,
		int period, int average,
		list_head_t(struct timeline_entry) *ref) {

  __indicator_super__(j, id, jtrend_feed);
  /* Our sub-indicators */
  roc_init(&j->roc, id, period, average);
  roc_init(&j->roc_ref, id, period, average);

  j->ref = ref;

  return 0;
}

void jtrend_release(struct jtrend *j) {
  
  __indicator_release__(j);
}
