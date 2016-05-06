/*
 * Cresus EVO - sim.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "sim.h"

int sim_init(struct sim *s, struct cluster *c) {

  s->cluster = c;
  return 0;
}

void sim_free(struct sim *s) {
  /* Nothing to do */
}

int sim_run(struct sim *s, sim_feed_ptr feed) {

  struct position *p;

  while(cluster_step(s->cluster)){
    /* First : check if there are pending positions */
    while(!slist_is_empty(&s->slist_position_pending)){
      /* FIXME */
      p = __slist_self__(s->slist_position_pending.next);
      slist_del(&s->slist_position_pending);
      /* Insert confirmed position */
      position_confirm(p);
      __slist_insert__(&s->slist_position_opened, p);
    }
    
    /* Second  : feed the sim */
    feed(s, s->cluster);
  }

  return 0;
}

int sim_add_position(struct sim *s, struct position *p) {

  __slist_insert__(&s->slist_position_pending, p);
  return 0;
}
