#include "jtrend.h"

static int jtrend_feed(struct indicator *i,
		       struct timeline_entry *e) {

  struct jtrend_entry *entry;
  struct jtrend *j = __indicator_self__(i);
  struct candle *c = __timeline_entry_self__(e);

  /* Do something here */
  return 0;
}

int jtrend_init(struct jtrend *j, indicator_id_t id,
		int period, int average) {

  __indicator_super__(j, id, jtrend_feed);
  /* TODO : populate */
  return 0;
}

void jtrend_release(struct jtrend *j) {
  
  __indicator_release__(j);
}
