/*
 * Cresus EVO - rs_mansfield.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "rs_mansfield.h"
#include "engine/candle.h"

/*
 * RSM = ((RSD(today) / mma(RSD(today), n)) - 1) * 100.0
 */

static int rs_mansfield_feed(struct indicator *i, struct timeline_entry *e) {

  struct timeline_entry *entry;
  struct candle *c = __timeline_entry_self__(e);
  struct rs_mansfield *r = __indicator_self__(i);

  if(i->is_empty)
    r->ref = __list__(e);

  if((entry = timeline_entry_find(__list_self__(r->ref), e->time))){
    struct candle *cref = __timeline_entry_self__(entry);
    double rsd = c->close / cref->close;
    double last = average_value(&r->mma);
    double mma = average_update(&r->mma, rsd);
    if(average_is_available(&r->mma)){
      /* Finally set value */
      double value = ((rsd / mma) - 1) * 100.0;
      struct rs_mansfield_indicator_entry *rsm;
      rsm = rs_mansfield_indicator_entry_alloc(value, (mma - last));
      candle_add_indicator_entry(c, __indicator_entry__(rsm));
    }
    
    /* Set new ref */
    r->ref = __list__(entry);
    return 0;
  }
  
  return -1;
}

int rs_mansfield_init(struct rs_mansfield *r, int period,
		      __list_head__(struct timeline_entry) *ref) {

  __indicator_super__(r,  rs_mansfield_feed);
  __indicator_set_string__(r, "rsm[%d]", period);
  
  average_init(&r->mma, AVERAGE_MATH, period);
  
  r->ref = ref;
  r->value = 0.0;
  
  return 0;
}

void rs_mansfield_free(struct rs_mansfield *r) {

  __indicator_free__(r);
  average_free(&r->mma);
}

/* Indicator-specific */
double rs_mansfield_value(struct rs_mansfield *r) {

  return average_value(&r->mma);
}
