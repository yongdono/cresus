/*
 * Cresus EVO - candle.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "candle.h"

int candle_init(struct candle *c,
		time_info_t time, granularity_t g,
		double open, double close,
		double high, double low,
		double volume) {
  
  /* superclass */
  __timeline_entry_super__(c, time, g);
  
  /* Set */
  c->open = open;
  c->close = close;
  c->high = high;
  c->low = low;
  c->volume = volume;

  /* Indicators */
  __slist_head_init__(&c->slist_indicator);
  
  return 0;
}

void candle_release(struct candle *c) {
  
  __timeline_entry_release__(c);
  __slist_head_release__(&c->slist_indicator);
}

void candle_add_indicator_entry(struct candle *c,
				struct indicator_entry *e) {

  __slist_insert__(&c->slist_indicator, e);
}

double candle_get_value(const struct candle *c, candle_value_t value) {
  
  switch(value) {
    case CANDLE_OPEN : return c->open;
    case CANDLE_CLOSE : return c->close;
    case CANDLE_HIGH : return c->high;
    case CANDLE_LOW : return c->low;
    case CANDLE_VOLUME : return c->volume;
    case CANDLE_TYPICAL : return (c->high + c->low + c->close) / 3.0;
    case CANDLE_WEIGHTED : return (c->high + c->low + 2 * c->close) / 4.0;
    case CANDLE_MEDIAN : return (c->high + c->low) / 2.0;
    case CANDLE_TOTAL : return (c->high + c->low + c->open + c->close) / 4.0;
  }
  
  /* Unknown */
  fprintf(stderr, "candle_get_info : bad value requested (%d)", value);
  return 0.0;
}

int candle_get_direction(const struct candle *c)
{
  /*
  if(c->open > c->close) return -1;
  if(c->open < c->close) return 1;
  */
  return (c->close - c->open);
}

/* Debug */

const char *candle_str(struct candle *c) {

  char buf[256];
  sprintf(c->str, "%s o%.1f c%.1f h%.1f l%.1f v%.0f",
	  __timeline_entry_str__(c, buf, sizeof buf),
	  c->open, c->close, c->high, c->low, c->volume);
	  
  return c->str;
}
