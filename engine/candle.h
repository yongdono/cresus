/*
 * Cresus EVO - candle.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef CANDLE_H
#define CANDLE_H

#include <float.h>
#include <stdlib.h>

#include "framework/slist.h"
#include "framework/input.h"

typedef enum {
  CANDLE_OPEN,
  CANDLE_CLOSE,
  CANDLE_HIGH,
  CANDLE_LOW,
  CANDLE_VOLUME,
  CANDLE_TYPICAL,
  CANDLE_WEIGHTED,
  CANDLE_MEDIAN,
  CANDLE_TOTAL
  /* Other info here */
} candle_value_t;

/* Object is allocatable */
#define candle_alloc(ctx, inptu_entry)				\
  DEFINE_ALLOC(struct candle, ctx, candle_init, input_entry)
#define candle_free(ctx)			\
  DEFINE_FREE(ctx, candle_release)

#define candle_is_green(ctx) ((ctx)->close >= (ctx)->open)
#define candle_is_red(ctx) ((ctx)->close < (ctx)->open)

struct candle {
  /* Inherits from slist cause we want this to be "stackable"
   * in a timeline_entry object */
  __inherits_from__(struct slist);
  
  /* Content */
  double open, close;
  double high, low;
  double volume;

  slist_head_t(struct indicator) slist_indicator;
};

int candle_init(struct candle *ctx,
		time_info_t time, time_gr_t g,
		double open, double close,
		double high, double low,
		double volume);
void candle_release(struct candle *ctx);

void candle_merge(struct candle *ctx, struct candle *c2);
double candle_get_value(struct candle *ctx, candle_value_t value);
int candle_get_direction(struct candle *ctx);

/*
static inline void *
__candle_find_indicator_entry__(struct candle *c, unique_id_t id)
{
  struct indicator_entry *entry = candle_find_indicator_entry(c, id);
  return (entry ? __indicator_entry_self__(entry) : NULL);
}
*/

/* Debug */
const char *candle_str(struct candle *c);
const char *candle_str_r(struct candle *c, char *buf);

#endif
