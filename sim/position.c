/*
 * Cresus EVO - position.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04.05.2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "position.h"

int position_init(struct position *p, struct timeline *t,
		  position_t type, int n) {

  /* super() */
  __slist_super__(p);
  
  p->t = t;
  p->in = NULL;
  p->out = NULL;
  
  p->n = n;
  p->type = type;
  
  return 0;
}

void position_release(struct position *p) {

  __slist_release__(p);
  p->in = NULL;
  p->out = NULL;
}

/* FIXME : find better names */

static int position_exec(struct position *p, struct candle **c) {

  struct timeline_entry *entry;
  int ret = timeline_entry_current(p->t, &entry);
  *c = __timeline_entry_self__(entry);
  
  return ret;
}

int position_in(struct position *p) {

  return position_exec(p, &p->in);
}

int position_out(struct position *p) {

  return position_exec(p, &p->out);
}
