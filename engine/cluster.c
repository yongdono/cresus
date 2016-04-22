/*
 * Cresus EVO - cluster.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 22/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "cluster.h"
#include "candle.h"

int cluster_init(struct cluster *c, const char *name) {

  /* Super */
  __timeline_super__(c, name, NULL); /* FIXME : still inherit ? */
  __slist_head_init__(&c->slist_timeline);
  
  return 0;
}

void cluster_free(struct cluster *c) {

}

int cluster_add_timeline(struct cluster *c, struct timeline *t) {

  __slist_insert__(&c->slist_timeline, t);
  return 0;
}

int cluster_step(struct cluster *c) {

  struct timeline *t;
  __slist_for_each__(&c->slist_timeline, t){
    /* Step all timelines */
    struct timeline_entry *entry;
    if((entry = timeline_step(t))){
      struct indicator_entry *indicator;
      struct candle *candle = __timeline_entry_self__(entry);
      __slist_for_each__(&candle->slist_indicator, indicator){
	/* Parse indicators */
	/* TODO : Use function pointer ? */
      }
    }
  }
  
  return 0;
}
