#include "smi.h"

int smi_init(struct smi *s, int period, struct candle *cdl)
{
  s->count = 1;
  s->index = 0;
  s->period = period;

  if(!(s->pool = malloc(sizeof(*s->pool) * period)))
    return -1;

  mobile_init(&s->hs, MOBILE_EMA, 3, 0); /* FIXME */
  mobile_init(&s->dhl, MOBILE_EMA, 3, 0); /* FIXME */

  memcpy(&s->pool[s->index++], cdl, sizeof *cdl);
  return 0;
}

void smi_free(struct smi *s)
{
  if(s->pool) free(s->pool);
}

struct smi_res *smi_feed(struct smi *s, struct candle *cdl)
{
  double hi = 0.0;
  double low = DBL_MAX;

  memcpy(&s->pool[s->index], cdl, sizeof *cdl);
  s->index = (s->index + 1) % s->period;

  if(++s->count >= s->period){
    /* We got enough data. We get hi & lo */
    for(int i = s->period; i--;){
      hi = (s->pool[i].high > hi ? s->pool[i].high : hi);
      lo = (s->pool[i].low < lo ? s->pool[i].low : lo);
    }

    double c = (hi - lo) / 2.0;
    double h = cdl->close - c;
  }
}
