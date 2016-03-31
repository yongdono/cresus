#include <math.h>
#include <stdio.h>
#include <string.h>

#include "macd.h"

int macd_init(struct macd *m, int fast_p, int slow_p,
              int signal_p, struct candle *seed)
{
  mobile_init(&m->fast, MOBILE_EMA, fast_p, seed);
  mobile_init(&m->slow, MOBILE_EMA, slow_p, seed);
  mobile_init(&m->signal, MOBILE_EMA, signal_p, 0); /* FIXME ? */

  memset(&m->result, 0, sizeof m->result);
  return 0;
}

void macd_free(struct macd *m)
{
  mobile_free(&m->fast);
  mobile_free(&m->slow);
  mobile_free(&m->signal);
}

struct macd_result *macd_feed(struct macd *m, struct candle *cdl)
{
  double fast = mobile_feed(&m->fast, cdl->close);
  double slow = mobile_feed(&m->slow, cdl->close);
  
  m->result.value = fast - slow;
  m->result.signal = mobile_feed(&m->signal, m->result.value);
  m->result.histogram = m->result.value - m->result.signal;

  return &m->result;
}

struct macd_result *macd_compute(struct macd *m)
{
  return &m->result;
}

const char *macd_str(struct macd *m, char *buf, size_t len)
{
  sprintf(buf, "%.2lf %.2lf (%+.2lf)", m->result.value, m->result.signal,
	  m->result.histogram);

  return buf;
}
