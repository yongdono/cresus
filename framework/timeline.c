#include "some_data_structures.h"

static struct timeline_slice *
timeline_get_slice(struct timeline *ctx, time_info_t time, time_gr_t gr)
{
  struct timeline_slice *ptr;
  
  /* TODO : Remember last position ? */
  __list_for_each__(&ctx->by_slice, ptr){
    time_info_t cmp = TIMECMP(time, ptr->time, gr);
    /* Slice already exists, we go out */
    if(!cmp) goto out;
    /* Slice is ahead, sort */
    if(cmp > 0){
      struct timeline_slice *slice;
      timeline_slice_alloc(slice, time);
      __list_add__(ptr, slice);
      goto out;
    }
  }
  
  /* Slice doesn't exist, we create it */
  timeline_slice_alloc(ptr, time);
  __list_add_tail__(&ctx->by_slice, ptr);
  
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
    if((slice = timeline_get_slice(ctx, input_entry->time,
                                   input_entry->gr)) != NULL){
      /* 3) Create track entry, register slice */
      timeline_track_entry_alloc(track_entry, input_entry, track, slice); /* ! */
      __list_add_tail__(&track->list_entries, track_entry); /* FIXME : sort ? */
      /* 4) Create slice entry & register track entry */
      timeline_slice_entry_alloc(slice_entry, track->uid, track_entry); /* ! */
      __list_add_tail__(slice, slice_entry);
    }
  }
}

int timeline_run_and_sync(struct timeline *ctx)
{
  /* Run indicators here */
  return 0;
}
