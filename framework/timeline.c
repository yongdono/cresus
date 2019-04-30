/*
 * Cresus EVO - timeline.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "framework/verbose.h"
#include "framework/timeline.h"

int timeline_init(struct timeline *ctx, const char *name)
{  
  /* Inheritance */
  __slist_super__(ctx);

  /* Data */
  strncpy(ctx->name, name, sizeof(ctx->name));
  list_head_init(&ctx->list_entry);
  slist_head_init(&ctx->slist_indicator);
  
  /* Parsing data */
  ctx->current = (void*)&ctx->list_entry;
  
  return 0;
}

void timeline_release(struct timeline *ctx)
{
  list_head_release(&ctx->list_entry);
  slist_head_release(&ctx->slist_indicator);
}

void timeline_add_indicator(struct timeline *ctx, struct indicator *i)
{
  __slist_insert__(&ctx->slist_indicator, i);
}

void timeline_load(struct timeline *ctx, struct input *in)
{
  struct indicator *indicator;
  struct timeline_entry *entry;
  
  while((entry = input_read(in))){
    /* Append entry to timeline */
    list_add_tail(&ctx->list_entry, __list__(entry));
    /* Execute indicators */
    __slist_for_each__(&ctx->slist_indicator, indicator){
      indicator_feed(indicator, entry);
      PR_DBG("%s feed indicator %s\n", ctx->name, indicator->str);
    }
  }
}

struct timeline_entry *timeline_step(struct timeline *ctx)
{
  ctx->current = __list_self__(__list__(ctx->current)->next);
  return ctx->current;
}

int timeline_entry_current(struct timeline *ctx, struct timeline_entry **e)
{
  *e = ctx->current;
  return (!e ? -1 : 0);
}
