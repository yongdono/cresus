/*
 * Cresus CommonTL - timeline.c
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 06/05/2019
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#include <stdio.h>

#include "framework/verbose.h"
#include "framework/timeline.h"

static char buf[256];

/*
 * Timeline track object
 */
const char *timeline_track_entry_str(struct timeline_track_entry *ctx)
{
  return timeline_track_entry_str_r(ctx, buf);
}

const char *timeline_track_entry_str_r(struct timeline_track_entry *ctx,
                                       char *buf)
{
  sprintf(buf, "%s " input_entry_interface_fmt,
          time_info2str(ctx->slice->time, GR_DAY), /* ! */
          input_entry_interface_args(ctx));
  
  return buf;
}

/*
 * Timeline slice object
 */
struct timeline_track_entry*
timeline_slice_track_entry_by_uid(struct timeline_slice *ctx, unique_id_t uid)
{
  struct timeline_slice_entry *slice_entry;
  
  if(!(slice_entry = (struct timeline_slice_entry*)
       __slist_by_uid_find__(&ctx->slist_slice_entries, uid)))
    return NULL;
  
  /* Finally get sync ref track entry */
  return slice_entry->track_entry;
}

/*
 * Timeline object
 */
static struct timeline_slice *
timeline_get_slice(struct timeline *ctx, time_info_t time)
{
  struct timeline_slice *ptr;
  
  /* TODO : Remember last position ? */
  __list_for_each__(&ctx->by_slice, ptr){
    time_info_t cmp = TIMECMP(ptr->time, time, GR_DAY); /* ! */
    /* Slice already exists, we go out */
    if(!cmp){
      PR_DBG("timeline.c: slice already exists\n");
      goto out;
    }
    /* Slice is ahead, sort */
    if(cmp > 0){
      struct timeline_slice *slice;
      timeline_slice_alloc(slice, time);
      __list_add__(ptr, slice);
      PR_DBG("timeline.c: slice is missing, insertion\n");
      goto out;
    }
  }
  
  /* Slice doesn't exist, we create it */
  timeline_slice_alloc(ptr, time);
  __list_add_tail__(&ctx->by_slice, ptr);
  PR_DBG("timeline.c: new slice at %s\n", time_info2str(time, GR_DAY));
  
 out:
  return ptr;
}

int timeline_init(struct timeline *ctx)
{
  list_head_init(&ctx->by_slice);
  slist_head_init(&ctx->by_track);
  return 0;
}

void timeline_release(struct timeline *ctx)
{
  list_head_release(&ctx->by_slice);
  slist_head_release(&ctx->by_track);
}

int timeline_add_track(struct timeline *ctx,
		       struct timeline_track *track,
		       struct input *input)
{
  struct timeline_slice *slice;
  struct input_entry *input_entry;
  struct timeline_track_entry *track_entry;
  struct timeline_slice_entry *slice_entry;

  /* 0) Add track to timeline */
  __slist_insert__(&ctx->by_track, track);
  
  /* 1) Read input */
  while((input_entry = input_read(input)) != NULL){
    /* 2) Create slice if necessary & sort it */
    PR_DBG("2) Create slice if necessary & sort it\n");
    if((slice = timeline_get_slice(ctx, input_entry->time)) != NULL){
      /* 3) Create track entry, register slice */
      PR_DBG("3) Create track entry, register slice\n");
      timeline_track_entry_alloc(track_entry, input_entry, track, slice); /* ! */
      __list_add_tail__(&track->list_track_entries, track_entry); /* FIXME : sort ? */
      /* 4) Create slice entry & register track entry */
      PR_DBG("4) Create slice entry & register track entry\n");
      timeline_slice_entry_alloc(slice_entry, track->uid, track_entry); /* ! */
      __slist_insert__(&slice->slist_slice_entries, slice_entry);
      PR_DBG("5) Back to 1\n");
    }
  }
}

int timeline_run_and_sync(struct timeline *ctx)
{
  /* Run indicators here */
  return 0;
}
