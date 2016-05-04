/*
 * Cresus EVO - position.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04.05.2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "position.h"

int position_init(struct position *p, struct timeline *t, int n) {

  /* super() */
  __slist_super__(p);
  
  p->t = t;
  p->c = NULL;
  p->n = n;
  
  return 0;
}

void position_release(struct position *p) {

  __slist_release__(p);
  p->c = NULL;
}

/* FIXME */
int position_confirm(struct position *p) {

  return timeline_entry_current(p->t, &p->c);
}
