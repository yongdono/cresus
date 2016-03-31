#include <math.h>
#include <stdio.h>
#include <string.h>

#include "atr.h"

int atr_init(struct atr *a, int period, struct candle *cdl)
{
  a->period = period;
  a->value = cdl->high - cdl->low;

  memcpy(&a->last, cdl, sizeof *cdl);
  return 0;
}

void atr_free(struct atr *a)
{
}

double atr_feed(struct atr *a, struct candle *cdl)
{
  double max = cdl->high - cdl->low;
  double h = fabs(cdl->high - a->last.close);
  double l = fabs(cdl->low - a->last.close);

  max = (h > max ? h : max);
  max = (l > max ? l : max);
  a->value = (a->value * (a->period - 1) + max) / a->period;

  /* Remember last candle */
  memcpy(&a->last, cdl, sizeof *cdl);
  return a->value;
}

double atr_compute(struct atr *a)
{
  return a->value;
}
