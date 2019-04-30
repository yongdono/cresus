/*
 * Cresus EVO - timeline_entry.c
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "framework/verbose.h"
#include "framework/indicator.h"
#include "framework/timeline_entry.h"

int timeline_entry_init(struct timeline_entry *ctx,
                        time_info_t time, granularity_t g)
{  
  /* Superclass */
  __list_super__(ctx);
  
  /* Internals */
  ctx->time = time;
  ctx->granularity = g;
  
  /* Indicators */
  slist_head_init(&ctx->slist_ientry);
  
  return 0;
}

void timeline_entry_release(struct timeline_entry *ctx)
{  
  __list_release__(ctx);
}

void timeline_entry_add_indicator_entry(struct timeline_entry *ctx,
                                        struct indicator_entry *ientry)
{
  __slist_insert__(&ctx->slist_ientry, ientry);
}

struct indicator_entry *
timeline_entry_find_indicator_entry(struct timeline_entry *ctx,
                                    unique_id_t indicator_id)
{  
  struct indicator_entry *ientry;
  
  __slist_for_each__(&ctx->slist_ientry, ientry){
    if(ientry->parent->id == indicator_id)
      return ientry;
  }
  
  PR_DBG("can't find any indicator %d entry\n", indicator_id);
  return NULL;
}

time_info_t timeline_entry_timecmp(struct timeline_entry *ctx,
				   time_info_t time)
{  
  return TIMECMP(ctx->time, time, ctx->granularity);
}

static struct timeline_entry *
timeline_entry_find_forward(struct timeline_entry *ctx, time_info_t time)
{
  struct list *entry;
  
  for(entry = __list__(ctx)->next;
      entry != entry->head;
      entry = entry->next){
    /* Find forward */
    struct timeline_entry *cur = __list_self__(entry);
    if(!timeline_entry_timecmp(cur, time))
      return cur;
  }

  return NULL;
}

static struct timeline_entry *
timeline_entry_find_backwards(struct timeline_entry *ctx, time_info_t time)
{
  struct list *entry;
  
  for(entry = __list__(ctx)->prev;
      entry != entry->head;
      entry = entry->prev){
    /* Find forward */
    struct timeline_entry *cur = __list_self__(entry);
    if(!timeline_entry_timecmp(cur, time))
      return cur;
  }

  return NULL;
}

struct timeline_entry *
timeline_entry_find(struct timeline_entry *ctx, time_info_t time)
{  
  time_info_t cmp;
  if(!(cmp = timeline_entry_timecmp(ctx, time)))
    /* time is the same */
    goto out;
  
  if(cmp < 0)
    return timeline_entry_find_forward(ctx, time);
  /* else */
  return timeline_entry_find_backwards(ctx, time);
  
 out:
  return ctx;
}

/* Debug functions */

static char str[256];

const char *timeline_entry_str(struct timeline_entry *ctx)
{
  return timeline_entry_str_r(ctx, str, sizeof(str));
}

const char *timeline_entry_str_r(struct timeline_entry *ctx,
				 char *buf, size_t len)
{
  /* FIXME : use len */
  return time_info2str_r(ctx->time, ctx->granularity, buf);
}
