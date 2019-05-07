/*
 * Cresus EVO - rs_roc.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 15/10/2018
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "rs_roc.h"
#include "framework/verbose.h"

static int rs_roc_feed(struct indicator *i, struct timeline_track_entry *e)
{
  double value, roc, roc_ref;
  struct rs_roc_entry *entry;

  struct rs_roc *ctx = (void*)i;
  struct timeline_track_entry *ref;
  struct timeline_track_entry *last;
  struct timeline_track_entry *ref_last;
  
  /* Get relative entry */
  if(!(last = __list_relative__(e, -(ctx->period))))
    goto out;
  /* Get synced track_entry */
  if(!(ref = timeline_slice_get_track_entry(e->slice, ctx->ref_track_uid)))
    goto out;
  /* Get relative entry to ext synced */
  if(!(ref_last = __list_relative__(ref, -(ctx->period))))
    goto out;
  
  /* ROC formula :
   * ((candle[n] / candle[n - period]) - 1) * 100.0
   */
  roc = (e->close / last->close - 1) * 100.0;
  roc_ref = (ref->close / ref_last->close - 1) * 100.0;
  value = roc - roc_ref;
    
  if(rs_roc_entry_alloc(entry, i, value, roc, roc_ref)){
    timeline_track_entry_add_indicator_entry(e, __indicator_entry__(entry));
    return 0;
  }
  
 out:
  return -1;
}

static void rs_roc_reset(struct indicator *i)
{
}

int rs_roc_init(struct rs_roc *ctx, unique_id_t uid, int period,
                unique_id_t ref_track_uid)
{
  /* Super() */
  __indicator_init__(ctx, uid, rs_roc_feed, rs_roc_reset);
  __indicator_set_string__(ctx, "rs_roc[%d]", period);

  ctx->ref_track_uid = ref_track_uid;
  ctx->period = period;
  return 0;
}

void rs_roc_release(struct rs_roc *ctx)
{
  __indicator_release__(ctx);
}
