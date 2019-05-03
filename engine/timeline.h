#ifndef TIMELINE_H
#define TIMELINE_H

#include "framework/list.h"
#include "framework/slist.h"

#include "engine/candle.h"

strcut timeline_entry {
  /* Can be parser either way */
  __inherits_from__(struct list);
  /* Time + any candle */
  time_info_t time;
  slist_head_t(struct candle) list_candle;
};

struct timeline {
  list_head(struct timeline_entry) list_entry;
};

#endif
