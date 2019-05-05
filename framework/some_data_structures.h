#ifndef SOME_DATA_STRUCTURES_H
#define SOME_DATA_STRUCTURES_H

#include "framework/list.h"
#include "framework/slist.h"
#include "framework/input.h"

/*
 * Order of operation :
 * 1) Read input
 * 2) Create slice if necessary. Sort them
 * 3) Create track entry. Append to previous. Register slice
 * 4) Create slice entry. Register track entry
 * 5) Back to 1
 * 6) Play indicators once slices are sync
 */

/* Required for compilation */
struct timeline_slice;

/*
 * Linear access
 */
struct timeline_track_entry {
  /* Can be parsed either way */
  __inherits_from__(struct list);
  /* Internal data */
  __implements__(input_entry_interface);
  /* Where are we from ? */
  struct timeline_track *owner; /* Who created this */
  struct timeline_slice *slice; /* By-slice sorting */
  /* More data, append later */
  slist_head_t(struct indicator_entry) indicator_entry;
};

static inline int
timeline_track_entry_init(struct timeline_track_entry *ctx,
			  struct input_entry *entry,
			  struct timeline_track *owner,
			  struct timeline_slice *slice)
{
  __list_init__(ctx); /* super() */
  input_entry_interface_copy(ctx, entry);
  ctx->owner = owner;
  ctx->slice = slice;
  return 0;
}

#define timeline_track_entry_alloc(ctx, input_entry,			\
				   track, slice)			\
  DEFINE_ALLOC(struct timeline_track_entry, ctx,			\
	       timeline_track_entry_init, input_entry,			\
	       track, slice)

struct timeline_track {
  /* It's a slist of lists */
  __inherits_from__(struct slist);
  /* Should have an id */
  unique_id_t track_id;
  /* Here's the beginning of the track */
  list_head_t(struct timeline_track_entry) list_entries;
  /* The indicators we want to play on that particular track */
  slist_head_t(struct indicator) slist_indicators;
};

static inline int
timeline_track_init(struct timeline_track *ctx,
		    unique_id_t track_id)
{
  __slist_init__(ctx); /* super() */
  ctx->track_id = track_id;
  return track_id;
}

#define timeline_track_alloc(ctx, track_id)			\
  DEFINE_ALLOC(struct timeline_track, ctx, timeline_track_init, \
	       track_id)

int timeline_track_add_indicator(struct timeline_track *ctx,
				 struct indicator *indicator);

/*
 * Access by slice / indice / time
 */
struct timeline_slice_entry {
  /* Can be parsed either way ? */
  __inherits_from__(struct slist);
  /* Should be easy to find (track_entry->owner->track_id ?) */
  struct timeline_track_entry *track_entry;
};

static inline int
timeline_slice_entry_init(struct timeline_slice_entry *ctx,
			  unique_id_t unique_id,
			  struct timeline_entry_track *entry)
{
  __list_init__(ctx); /* super() */
  ctx->unique_id = unique_id;
  track_entry = entry;
  return 0;
}

struct timeline_slice {
  __inherits_from__(struct list);
  /* It's a time slice */
  time_info_t time_info;
  /* Containing one or more timeline_slices_entries */
  slist_head_t(struct timeline_slice_entry) slist_entries;
};

/* The final object */
struct timeline {
  struct timeline_track by_track;
  struct timeline_slice by_slice;
};

/* Interfaces */
int timeline_init(struct timeline *ctx);
void timeline_release(struct timeline *ctx);

int timeline_add_track(struct timeline *ctx,
		       struct timeline_track *track,
		       struct input *input);

/* Run & sync all */
int timeline_run_and_sync(struct timeline *ctx);

#endif
