#ifndef SOME_DATA_STRUCTURES_H
#define SOME_DATA_STRUCTURES_H

#include "framework/list.h"
#include "framework/slist.h"
#include "framework/input.h"

/*
 * Linear access
 */
struct timeline_entry {
  /* Can be parsed either way */
  __inherits_from__(struct list);
  /* Internal data */
  double open, close, high, low, volume; /* input_entry_interface ? */
  slist_head_t(struct indicator_entry) indicator_entry;
};

/*
 * Access by slice / indice / time
 */
struct timeline_slice_entry {
  /* Can be parsed either way ? */
  __inherits_from__(struct slist);
  struct timeline_entry *entry;
};

struct timeline_slice {
  __inherits_from__(struct list);
  /* It's a time slice */
  time_info_t time_info;
  /* Containing one or more timeline_slices_entries */
  slist_head_t(struct timeline_slice_entry) slist_entries;
};

strcut timeline {
  list_head_t(struct timeline_slice) list_slices;
};

/* What about indicators ? */

#endif
