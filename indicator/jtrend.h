#ifndef JTREND_H
#define JTREND_H

#include "math/average.h"
#include "engine/candle.h"
#include "framework/alloc.h"
#include "framework/indicator.h"

struct jtrend_entry {
  /* Always */
  __inherits_from_indicator_entry__;
  /* Value type ? */
};

#define jtrend_entry_alloc(entry, parent)				\
  DEFINE_ALLOC(struct jtrend_entry, entry, jtrend_entry_init, parent)
#define jtrend_entry_free(entry)		\
  DEFINE_FREE(entry, jtrend_entry_release)

static inline jtrend_entry_init(struct jtrend_entry *entry,
				struct indicator *parent){
  __indicator_entry_super__(entry, parent);
  return 0;
}

static inline jtrend_entry_release(struct jtrend_entry *entry) {
  __indicator_entry_release__(entry);
}

/* Main object */

struct jtrend {
  /* Anyway */
  __inherits_from_indicator__;
  /* Some other data */
};

int jtrend_init(struct jtrend *j, indicator_id_t id,
		int period, int average);

void jtrend_release(struct jtrend *j);

#endif
