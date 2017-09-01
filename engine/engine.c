/*
 * Cresus EVO - engine.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 09/01/2017
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#include <stdio.h>
#include "engine.h"

int engine_init(struct engine *e, struct timeline *t) {

  e->timeline = t;
  /* Init slists */
  list_head_init(&e->list_position_to_open);
  list_head_init(&e->list_position_opened);
  list_head_init(&e->list_position_to_close);
  list_head_init(&e->list_position_closed);
  
  /* Stat */
  e->factor = 1.0;
  e->nwin = 0;
  e->nloss = 0;
  
  return 0;
}

void engine_release(struct engine *e) {
  /* Nothing to do */
  list_head_release(&e->list_position_to_open);
  list_head_release(&e->list_position_opened);
  list_head_release(&e->list_position_to_close);
  list_head_release(&e->list_position_closed);
}

static void engine_xfer_positions(struct engine *e,
				  list_head_t(struct position) *dst,
				  list_head_t(struct position) *src,
				  position_action_t position_action) {
  
  struct list *safe;
  struct position *p;
  
  __list_for_each_safe__(src, p, safe){
    __list_del__(p);
    /* Insert in dst */
    __list_add_tail__(dst, p);
    /* Act on position */
    position_action(p);
  }
}

void engine_run(struct engine *e, engine_feed_ptr feed) {

  struct timeline_entry *entry;
  while((entry = timeline_step(e->timeline)) != NULL){
    /* First : check if there are opening positions */
    engine_xfer_positions(e, &e->list_position_opened,
			  &e->list_position_to_open,
			  position_in);
    /* Second : check if there are closing positions */
    engine_xfer_positions(e, &e->list_position_closed,
			  &e->list_position_to_close,
			  position_out);
    /* Third  : feed the engine */
    feed(e->timeline, entry);
  }
  
  return 0;
}

int engine_open_position(struct engine *e, struct timeline *t, 
			 position_t type, double n) {
  
  struct position *p;
  if(position_alloc(p, t, type, n)){
    __list_add_tail__(&e->list_position_to_open, p);
    return 0;
  }

  return -1;
}

int engine_close_position(struct engine *e, struct timeline *t) {
  
  int n = 0;
  struct list *safe;
  struct position *p;
  
  __list_for_each_safe__(&e->list_position_opened, p, safe){
    if(p->t == t){ /* FIXME : use timeline's name ? */
      __list_del__(p);
      __list_add_tail__(&e->list_position_to_close, p);
      n++;
    }
  }
  
  return n;
}

int engine_close_all_positions(struct engine *e) {
  
  engine_xfer_positions(e, &e->list_position_to_close,
			&e->list_position_opened,
			position_nop);
  
  return 0;
}
