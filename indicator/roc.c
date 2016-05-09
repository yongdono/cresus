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
  
  if(!i->is_empty){

    double value, average;
    struct roc_entry *entry;
    
    if(roc_compute(r, e, &value, &average) != -1){
      if(roc_entry_alloc(entry, i, value, average)){
	candle_add_indicator_entry(c, __indicator_entry__(entry));
	return 1;
      }
    }
  }
  
  return 0;
}

int roc_compute(struct roc *r, struct timeline_entry *e,
		double *rvalue, double *raverage) {

  struct candle *c = __timeline_entry_self__(e);
  struct timeline_entry *ref = __list_relative__(e, -(r->period));
  
  if(ref != NULL){
    /* ROC formula :
     * ((candle[n] / candle[n - period]) - 1) * 100.0
     */
    struct candle *cref = __timeline_entry_self__(ref);
    double value = ((c->close / cref->close) - 1) * 100.0;
    double average = average_update(&r->average, value);
    
    if(average_is_available(&r->average)){
      *rvalue = value;
      *raverage = average;
      return 0;
    }
  }
  
  return -1;
}

int roc_init(struct roc *r, indicator_id_t id, int period, int average) {

  /* Super() */
  __indicator_super__(r, id, roc_feed);
  __indicator_set_string__(r, "roc[%d,%d]", period, average);
  
  r->period = period;
  average_init(&r->average, AVERAGE_MATH, average);
  return 0;
}

void roc_release(struct roc *r)
{
  __indicator_release__(r);
  average_release(&r->average);
}
