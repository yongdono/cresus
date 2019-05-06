/*
 * Cresus EVO - hilo.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 12.25.2017
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */
#include "hilo.h"

static void _hilo_reset_(struct indicator *i)
{
  struct hilo *ctx =  (void*)i;
  // TODO
}

void hilo_reset(struct hilo *ctx)
{
  _hilo_reset_(__indicator__(ctx));
}

#define HIGH(x, y) (((x) > (y)) ? (x) : (y))
#define LOW(x, y) (((x) < (y)) ? (x) : (y))

static int hilo_feed(struct indicator *i, struct timeline_track_entry *e)
{
  struct hilo_entry *entry;
  struct hilo *ctx = (void*)i;
  struct timeline_track_entry *prev = NULL;
  
  if(hilo_entry_alloc(entry, i)){
    /* Init */
    int n = ctx->period;
    entry->high = e->high;
    entry->low = e->low;
    
    __list_for_each_prev__(e, prev){
      
      /* Out after ctx->period iterations */
      if(!--n)
	break;
      
      entry->high = HIGH(entry->high, prev->high);
      entry->low = LOW(entry->low, prev->low);
    }
    
    if(ctx->filter && n)
      goto out;
    
    /* Attach new entry */
    timeline_track_entry_add_indicator_entry(e, __indicator_entry__(entry));
    return 1;
  }

 out:
  return 0;
}

int hilo_init(struct hilo *ctx, unique_id_t id, int period, int filter)
{
  /* Super() */
  __indicator_init__(ctx, id, hilo_feed, _hilo_reset_);
  __indicator_set_string__(ctx, "hilo[%d]", period);
  
  ctx->period = period;
  ctx->filter = filter; /* bool to filter incomplete data */
  return 0;
}

void hilo_release(struct hilo *ctx)
{
  __indicator_release__(ctx);
}
