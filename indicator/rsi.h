#ifndef RSI_H
#define RSI_H

/*
 * Relative Strength Index
 * Momentum indicator
 * Typical period is 12
 */

#include "candle.h"
#include "mobile.h"

struct rsi {
  int index;
  int count;
  int period;

  double last;

  struct mobile mma_up;
  struct mobile mma_down;

  double up;
  double down;
};

int rsi_init(struct rsi *r, int period, struct candle *cdl);
void rsi_free(struct rsi *r);

int rsi_feed(struct rsi *r, struct candle *cdl);
double rsi_compute(struct rsi *r);

#endif
