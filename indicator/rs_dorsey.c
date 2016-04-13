/*
 * Cresus EVO - rs_dorsey.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "rs_dorsey.h"
#include "engine/candle.h"

static int rs_dorsey_feed(struct indicator *i, struct timeline_entry *e) {

  struct timeline_entry *entry;
  struct rs_dorsey *r = __indicator_self__(i);
  struct candle *c = __timeline_entry_self__(e);

  if(i->is_empty)
    r->ref = __list__(e);
  
  if((entry = timeline_entry_find(__list_self__(r->ref), e->time))){
    struct candle *cref = __timeline_entry_self__(entry);
    r->value = c->close / cref->close;
    /* Set new ref */
    r->ref = __list__(entry);
    /* TODO : create new entry ? */
    return 0;
  }
  
  return -1;
}

int rs_dorsey_init(struct rs_dorsey *r, indicator_id_t id,
		   __list_head__(struct timeline_entry) *ref) {

  /* super() */
  __indicator_super__(r, id, rs_dorsey_feed);
  __indicator_set_string__(r, "rsd[i]");

  r->ref = ref->next;
  return 0;
}

void rs_dorsey_free(struct rs_dorsey *r) {

  __indicator_free__(r);
}

/* Indicator-specific */
double rs_dorsey_value(struct rs_dorsey *r) {

  return r->value;
}
