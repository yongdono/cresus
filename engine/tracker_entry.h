/*
 * Cresus - tracker_entry.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 30/04/2019
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 * 
 */

#ifndef TRACKER_ENTRY_H
#define TRACKER_ENTRY_H

#include "framework/list.h"
#include "framework/time_info.h"

/* Object is allocatable */
#define tracker_entry_alloc(ctx, time)                                  \
  DEFINE_ALLOC(struct tracker_entry, ctx, tracker_entry_init, time)
#define tracker_entry_free(ctx)                 \
  DEFINE_FREE(ctx, tracker_entry_release)

struct tracker_entry {
  /* Super */
  __inherits_from_list__;
  /* Internals */
  time_info_t time;
  slist_head_t(struct timeline_entry) slist_tentry;
};

static inline int tracker_entry_init(struct tracker_entry *ctx,
                                     time_info_t time)
{
  __list_init__(ctx);
  ctx->time = time;
  slist_head_init(&ctx->slist_tentry);
  return 0;
}

static inline void tracker_entry_release(struct tracker_entry *ctx)
{
  __list_release__(ctx);
  slist_head_release(ctx->slist_tentry);
}

/* TODO : add timeline entry */

#endif
