#ifndef POSITION_H
#define POSITION_H

#include "engine/candle.h"
#include "engine/timeline.h"
#include "framework/slist.h"

struct position {
  /* slistable */
  __inherits_from_slist__;
  /* Some data */
  struct timeline *t;
  struct candle *c;
};

int position_init(struct position *p, struct timeline *t);
void position_release(struct position *p);

/* FIXME */
int position_confirm(struct position *p);

#endif
