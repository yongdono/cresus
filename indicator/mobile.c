#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mobile.h"

int mobile_init(struct mobile *m, mobile_t type,
                int period, candle_value_t value,
                const struct candle *seed)
{
  /* Super */
  indicator_init(&m->parent, value, mobile_feed);
  
  m->count = 1;
  m->index = 0;
  m->type = type;
  m->period = period;
  m->dir = MOBILE_DIR_UP; /* FIXME */
  m->pos = MOBILE_POS_BELOW;

  if(!(m->pool = malloc(sizeof(double) * period)))
    return -1;

  /* MMA */
  m->pool[m->index] = candle_get_value(seed, value);

  /* EMA */
  m->avg = candle_get_value(seed, value);
  m->k = 2.0 / (period + 1);

  return 0;
}

void mobile_free(struct mobile *m)
{
  indicator_free(&m->parent);
  if(m->pool) free(m->pool);
}

static void mobile_manage_direction(struct mobile *m, double avg,
                                    const struct candle *candle) {
  
  /* Check direction change */
  if(avg > m->avg){
    if(m->dir == MOBILE_DIR_DOWN)
      indicator_throw_event(&m->parent, EVENT_MOBILE_CHDIR_UP, candle);
    
    m->dir = MOBILE_DIR_UP;
    
  }else if(avg < m->avg){
    if(m->dir == MOBILE_DIR_UP)
      indicator_throw_event(&m->parent, EVENT_MOBILE_CHDIR_DOWN, candle);
    
    m->dir = MOBILE_DIR_DOWN;
  }
  
  /* If equals, keep last info */
}

static void mobile_manage_position(struct mobile *m, double avg,
                                   const struct candle *candle) {
  
  double candle_value = candle_get_value(candle, m->parent.value);
  if(avg > candle_value){
    if(m->pos == MOBILE_POS_BELOW)
      indicator_throw_event(&m->parent, EVENT_MOBILE_CROSSED_DOWN, candle);
    
    m->pos = MOBILE_POS_ABOVE;
    
  }else if(avg < candle_value) {
    if(m->pos == MOBILE_POS_ABOVE)
      indicator_throw_event(&m->parent, EVENT_MOBILE_CROSSED_UP, candle);
    
    m->pos = MOBILE_POS_BELOW;
  }
  
  /* If equals, do nothing */
}

int mobile_feed(struct indicator *i, const struct candle *candle)
{
  double avg;
  double sum = 0.0;
  struct mobile *m = (struct mobile*)i;
  double candle_value = candle_get_value(candle, i->value);
  
  m->index = (m->index + 1) % m->period; /* Inc */
  m->pool[m->index] = candle_value;
  m->count++;

  switch(m->type){
  case MOBILE_MMA :
    if(m->count >= m->period){
      for(int i = m->period; i--;)
        sum += m->pool[i];
      
      avg = sum / m->period;

    }else
      avg = 0.0;

    break;

  case MOBILE_EMA :
    avg = m->k * candle_value + (1 - m->k) * m->avg;
    break;
  }

  /* Check direction change */
  mobile_manage_direction(m, avg, candle);
  /* Check position change */
  mobile_manage_position(m, avg, candle);
  
  m->avg = avg;
  return m->avg;
}

double mobile_average(struct mobile *m)
{
  return m->avg;
}

double mobile_stddev(struct mobile *m)
{
  double sum = 0.0;

  if(m->count >= m->period){
    for(int i = m->period; i--;)
      sum += pow(m->pool[i] - m->avg, 2.0);
  }

  return sqrt(sum / m->period);
}

