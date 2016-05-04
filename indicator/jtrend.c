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

  struct jtrend_entry *entry;
  struct jtrend *j = __indicator_self__(i);
  struct candle *c = __timeline_entry_self__(e);

  if(!i->is_empty){

    /* jtrend formula :
     * average[n](roc[x][1](self) / roc[x][1](ref))
     */
    
    /* self ROC */
    //double roc = ((c->close / r->last->close) - 1) * 100.0;
    //double roc_ref = ((ref->close / r->ref_last->close) - 1) * 100.0;
    
  }
  
  //r->last = c;
  /* Do something here */
  return 0;
}

int jtrend_init(struct jtrend *j, indicator_id_t id,
		int period, int average) {

  __indicator_super__(j, id, jtrend_feed);
  /* TODO : populate */
  return 0;
}

void jtrend_release(struct jtrend *j) {
  
  __indicator_release__(j);
}
