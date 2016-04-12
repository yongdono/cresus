/*
 * Cresus EVO - rs_mansfield.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef RS_MANSFIELD_H
#define RS_MANSFIELD_H

/*
 * Relative Strength (Mansfield)
 * Fomula :
 * RSM = ((RSD(today) / mma(RSD(today), n)) - 1) * 100.0
 */

#include <stdlib.h>
#include "math/average.h"
#include "framework/list.h"
#include "framework/indicator.h"

/* Entries */

struct rs_mansfield_indicator_entry {
  /* As below */
  __inherits_from_indicator_entry__;
  /* Single value */
  double value;
  /* More info */
  double direction;
  /* Events ? */
};

static inline int
rs_mansfield_indicator_entry_init(struct rs_mansfield_indicator_entry *entry,
				  double value, double direction){
  __indicator_entry_super__(entry);
  entry->value = value;
  entry->direction = direction;
  return 0;
}

static inline struct rs_mansfield_indicator_entry *
rs_mansfield_indicator_entry_alloc(double value, double direction) {
  struct rs_mansfield_indicator_entry *entry;
  if((entry = malloc(sizeof *entry)))
    rs_mansfield_indicator_entry_init(entry, value, direction);
  
  return entry;
}

/* Object */

struct rs_mansfield {
  /* As always, inherits from indicator */
  __inherits_from_indicator__;
  
  struct average mma;
  __list_head__(struct timeline_entry) *ref;

  double value;
};

int rs_mansfield_init(struct rs_mansfield *r, int period,
		      __list_head__(struct timeline_entry) *ref);
void rs_mansfield_free(struct rs_mansfield *r);

/* Indicator-specific */
double rs_mansfield_value(struct rs_mansfield *r);

#endif
