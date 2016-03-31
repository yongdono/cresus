#include <math.h>
#include <stdlib.h>

#include "rsi.h"
#include "mobile.h"

int rsi_init(struct rsi *r, int period, struct candle *cdl)
{
  r->count = 0;
  r->index = 0;
  r->period = period;
  r->last = cdl->close;

  mobile_init(&r->mma_up, MOBILE_MMA, period, 0);
  mobile_init(&r->mma_down, MOBILE_MMA, period, 0);

  r->up = 0.0;
  r->down = 0.0;

  return 0;
}

void rsi_free(struct rsi *r)
{
  mobile_free(&r->mma_up);
  mobile_free(&r->mma_down);
}

int rsi_feed(struct rsi *r, struct candle *cdl)
{
  double val = cdl->close - r->last;

  if(r->count < r->period){
    if(val > 0) mobile_feed(&r->mma_up, val);
    if(val < 0) mobile_feed(&r->mma_down, fabs(val));
  
  }else if(r->count == r->period){
    r->up = mobile_average(&r->mma_up);
    r->down = mobile_average(&r->mma_down);
    
  }else{ /* r->count > r->period */
    if(val > 0) r->up = (r->up * (r->period - 1) + val) / r->period;
    if(val < 0) r->down = (r->down * (r->period - 1) + fabs(val)) / r->period;
  }
  
  r->last = cdl->close;
  return ++r->count;
}

double rsi_compute(struct rsi *r)
{
  /* double u = mobile_average(&r->up) / r->period; */
  /* double d = mobile_average(&r->down) / r->period; */

  double u = r->up / r->period;
  double d = r->down / r->period;

  if(r->count < r->period)
    return 0.0;

  return 100.0 - 100.0 / (1 + /* RS */ (u / d));
}
