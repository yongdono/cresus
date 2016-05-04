/*
 * Cresus EVO - roc.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 02.05.2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <math.h>
#include <stdlib.h>

#include "roc.h"

static int roc_feed(struct indicator *i, struct timeline_entry *e) {

  struct roc *r = __indicator_self__(i);
  struct candle *c = __timeline_entry_self__(e);
  struct candle *ref = __timeline_entry_relative_self__(e, -(r->period));
  
  if(!i->is_empty){
    
    /* ROC formula :
     * ((candle[n] / candle[n - period]) - 1) * 100.0
     */
    
    struct roc_entry *entry;
    double value = ((c->close / ref->close) - 1) * 100.0;
    double average = average_update(&r->average, value);
    
    if(average_is_available(&r->average)){
      if(roc_entry_alloc(entry, i, value, average)){
	candle_add_indicator_entry(c, __indicator_entry__(entry));
	return 1;
      }
    }
  }

  return 0;
}

int roc_init(struct roc *r, indicator_id_t id, int period, int average)
{
  /* Super() */
  __indicator_super__(r, id, roc_feed);
  __indicator_set_string__(r, "roc[%d %d]", period, average);
  
  r->period = period;
  average_init(&r->average, AVERAGE_MATH, period);
  return 0;
}

void roc_release(struct roc *r)
{
  __indicator_release__(r);
  average_release(&r->average);
}
