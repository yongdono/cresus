#ifndef MACD_H
#define MACD_H

#include "mobile.h"
#include "candle.h"

struct macd_result {
  double value;
  double signal;
  double histogram;
};

struct macd {
  struct mobile fast;
  struct mobile slow;
  struct mobile signal;

  /* Private */
  struct macd_result result;
};

int macd_init(struct macd *m, int fast_p, int slow_p, int signal_p,
	      struct candle *cdl);

void macd_free(struct macd *m);

struct macd_result *macd_feed(struct macd *m, struct candle *cdl);
struct macd_result *macd_compute(struct macd *m); /* FIXME ? */

const char *macd_str(struct macd *m, char *buf, size_t len);

#endif
