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
  list_init(&s->list_position_to_open);
  list_init(&s->list_position_opened);
  list_init(&s->list_position_to_close);
  list_init(&s->list_position_closed);

  return 0;
}

void sim_free(struct sim *s) {
  /* Nothing to do */
}

static void sim_xfer_positions(struct sim *s,
			       list_head_t(struct position) *dst,
			       list_head_t(struct position) *src,
			       position_action_t position_action) {
  
  struct position *p;
  __list_for_each__(src, p){
    __list_del__(p); /* FIXME : use safe ? */
    /* Insert in dst */
    __list_add_tail__(dst, p);
    /* Act on position */
    position_action(p);
  }
}

int sim_run(struct sim *s, sim_feed_ptr feed) {

  while(cluster_step(s->cluster)){
    /* First : check if there are opening positions */
    sim_xfer_positions(s, &s->list_position_opened,
		       &s->list_position_to_open,
		       position_in);
    /* Second : check if there are closing positions */
    sim_xfer_positions(s, &s->list_position_closed,
		       &s->list_position_to_close,
		       position_out);
    /* Third  : feed the sim */
    feed(s, s->cluster);
  }

  return 0;
}

int sim_open_position(struct sim *s, struct position *p) {

  __list_add__(&s->list_position_to_open, p);
  return 0;
}

int sim_close_position(struct sim *s, struct position *p) {

  /* FIXME : how to find position ? */
  return 0;
}

int sim_new_position(struct sim *s, struct timeline *t, 
		     position_t type, int n) {
  
  struct position *p;
  if(position_alloc(p, t, type, n)){
    __list_add__(&s->list_position_to_open, p); /* open could be removed */
    return 0;
  }

  return -1;
}

int sim_end_position(struct sim *s, struct timeline *t) {

  int n = 0;
  struct position *p;

  __list_for_each__(&s->list_position_opened, p){
    if(p->t == t){ /* FIXME : use timeline's name ? */
      __list_del__(p); /* FIXME : use safe */
      __list_add_tail__(&s->list_position_to_close, p);
      n++;
    }
  }

  return n;
}
