#ifndef SMI_H
#define SMI_H

/*
 * Stochastic Momentum Index
 */

#include "framework/candle.h"
#include "mobile.h"

struct smi_res {
  double value;
  double signal;
  double histogram;
};

struct smi {

  int count;
  int index;
  int period;
  struct candle *pool;

  struct smi_res res;
};

int smi_init(struct smi *s, int period, struct candle *cdl);
void smi_free(struct smi *s);

struct smi_res *smi_feed(struct smi *s, struct candle *cdl);
struct smi_res *smi_compute(struct smi *s);

#endif
