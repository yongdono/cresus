#ifndef ATR_H
#define ATR_H

#include "mobile.h"
#include "framework/candle.h"

struct atr {
  int period;
  double value;

  struct candle last;
};

int atr_init(struct atr *a, int period, struct candle *cdl);
void atr_free(struct atr *a);

double atr_feed(struct atr *a, struct candle *cdl);
double atr_compute(struct atr *a);

#endif
