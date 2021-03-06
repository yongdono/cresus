/*
 * Cresus EVO - mod_mma_simple.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/21/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef __Cresus_EVO__mod_mma_simple__
#define __Cresus_EVO__mod_mma_simple__

#include "module.h"
#include "mobile.h"
#include "stoploss.h"

struct mod_mma_simple {
  struct module parent;
  struct mobile mma;
  struct stoploss stoploss;
  
  int stock;
};

int mod_mma_simple_init(struct mod_mma_simple *m, const struct candle *seed);
void mod_mma_simple_release(struct mod_mma_simple *m);

#endif /* defined(__Cresus_EVO__mod_mma_simple__) */
