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
  /* Init slists */
  slist_init(&s->slist_position_to_open);
  slist_init(&s->slist_position_opened);
  slist_init(&s->slist_position_to_close);
  slist_init(&s->slist_position_closed);

  return 0;
}

void sim_free(struct sim *s) {
  /* Nothing to do */
}

static void sim_xfer_positions(struct sim *s,
			       slist_head_t(struct position) *dst,
			       slist_head_t(struct position) *src) {
  
  struct position *p;

  while(!slist_is_empty(src)){
    /* FIXME ? LIFO */
    p = __slist_self__(src->next);
    slist_del(src);
    /* Insert confirmed position */
    position_in(p);
    __slist_insert__(dst, p);
  }
}

int sim_run(struct sim *s, sim_feed_ptr feed) {

  while(cluster_step(s->cluster)){
    /* First : check if there are opening positions */
    sim_xfer_positions(s, &s->slist_position_opened,
		       &s->slist_position_to_open);
    /* Second : check if there are closing positions */
    sim_xfer_positions(s, &s->slist_position_closed,
		       &s->slist_position_to_close);
    /* Third  : feed the sim */
    feed(s, s->cluster);
  }

  return 0;
}

int sim_open_position(struct sim *s, struct position *p) {

  __slist_insert__(&s->slist_position_to_open, p);
  return 0;
}

int sim_close_position(struct sim *s, struct position *p) {

  /* FIXME : how to find position ? */
}

int sim_new_position(struct sim *s, struct timeline *t, 
		     position_t type, int n) {
  
  struct position *p;
  if(position_alloc(p, t, type, n)){
    sim_open_position(s, p);
    return 0;
  }

  return -1;
}
