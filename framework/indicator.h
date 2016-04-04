/*
 * Cresus EVO - indicator.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/21/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef __Cresus_EVO__indicator__
#define __Cresus_EVO__indicator__

#include "candle.h"

/* Define types */
struct indicator;
typedef int (*indicator_feed_ptr)(struct indicator*, const struct candle*);

struct indicator {
  candle_value_t value;
  indicator_feed_ptr feed;
};

int indicator_init(struct indicator *i,
                   candle_value_t value,
                   indicator_feed_ptr feed);

void indicator_free(struct indicator *i);

int indicator_feed(struct indicator *i, const struct candle *candle);
void indicator_set_event(struct indicator *i, struct candle *candle, int event);

#endif /* defined(__Cresus_EVO__indicator__) */
