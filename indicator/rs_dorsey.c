/*
 * Cresus EVO - rs_dorsey.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "rs_dorsey.h"

int rs_dorsey_init(struct rs_dorsey *r, struct candle *ref) {

  /* super() */
  __indicator_super__(r, rs_dorsey_feed);
  __indicator_set_string__(r, "rsd[]");

  r->ref = ref;
  return 0;
}

void rs_dorsey_free(struct rs_dorsey *r) {

  __indicator_free__(r);
}

int rs_dorsey_feed(struct indicator *i, const struct candle *c) {

  struct rs_dorsey *r = __indicator_self__(i);
  struct candle *ref = __timeline_entry_find__(r->ref,
					       __timeline_entry__(c).time,
					       1);
  
  if(ref){
    r->value = c->close / ref->close;
    /* TODO : create new entry ? */
    return 0;
  }
  
  return -1;
}

/* Indicator-specific */
double rs_dorsey_value(struct rs_dorsey *r) {

  return r->value;
}
