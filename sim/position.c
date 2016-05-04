#include "position.h"

int position_init(struct position *p, struct timeline *t) {

  /* super() */
  __slist_super__(p);
  
  p->t = t;
  p->c = NULL;
  
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
