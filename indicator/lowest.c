/*
 * Cresus EVO - lowest.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 08.05.2018
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */
#include "lowest.h"

static void _lowest_reset_(struct indicator *i)
{
  struct lowest *ctx =  (void*)i;
  /* Nothing to do */
}

void lowest_reset(struct lowest *ctx)
{
  _lowest_reset_(__indicator__(ctx));
}

static int lowest_feed(struct indicator *i,
		       struct timeline_entry *e)
{
  int n = 0;
  struct lowest_entry *entry;
  struct timeline_entry *prev = NULL;
  struct lowest *ctx = (void*)i;
  struct candle *p, *c = (void*)e;
  
  if(lowest_entry_alloc(entry, i)){
    /* Init entry value */
    entry->value = c->low;
    /* Find a lower value to exit */
    __list_for_each_prev__(c, p){
      entry->value = MIN(p->low, entry->value);
      /* End of loop */
      if(++n >= ctx->period)
	break;
    }
    
    /* Attach new entry */
    timeline_entry_add_indicator_entry(e, __indicator_entry__(entry));
    return 1;
  }
  
 out:
  return 0;
}

int lowest_init(struct lowest *ctx, unique_id_t id, int period)
{
  /* Super() */
  __indicator_init__(ctx, id, lowest_feed, _lowest_reset_);
  __indicator_set_string__(ctx, "lowest");
  /* Init internals */
  ctx->period = period;
  return 0;
}

void lowest_release(struct lowest *ctx)
{
  __indicator_release__(ctx);
}
