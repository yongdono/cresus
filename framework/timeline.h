/*
 * Cresus CommonTL - timeline.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 06/05/2019
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef TIMELINE_H
#define TIMELINE_H

#include "framework/list.h"
#include "framework/alloc.h"
#include "framework/slist.h"
#include "framework/input.h"
#include "framework/slist_by_uid.h"

/*
 * Order of operation :
 * 1) Read input
 * 2) Create slice if necessary. Sort it
 * 3) Create track entry. Append to previous. Register slice
 * 4) Create slice entry. Register track entry. Append to slice
 * 5) Back to 1
 * 6) Play indicators once slices are sync
 */

/* Required for compilation */
struct indicator;
struct timeline_slice;
struct indicator_entry;

/*
 * Linear access
 */
struct timeline_track_entry {
  /* Can be parsed either way */
  __inherits_from__(struct list);
  /* Internal data */
  __implements__(input_entry_interface);
  /* Where are we from ? */
  struct timeline_track *track; /* Who created this */
  struct timeline_slice *slice; /* By-slice sorting */
  /* More data, append later */
  slist_head_t(struct indicator_entry) slist_indicator_entries;
};

static inline int
timeline_track_entry_init(struct timeline_track_entry *ctx,
			  struct input_entry *entry,
			  struct timeline_track *track,
			  struct timeline_slice *slice)
{
  __list_init__(ctx); /* super() */
  input_entry_interface_copy(ctx, entry);
  ctx->track = track;
  ctx->slice = slice;
  slist_head_init(&ctx->slist_indicator_entries);
  return 0;
}

#define timeline_track_entry_alloc(ctx, input_entry, track, slice)      \
  DEFINE_ALLOC(struct timeline_track_entry, ctx,			\
	       timeline_track_entry_init, input_entry, track, slice)

/* Methods */
static inline void
timeline_track_entry_add_indicator_entry(struct timeline_track_entry *ctx,
                                         struct indicator_entry *entry)
{
  slist_insert(&ctx->slist_indicator_entries, __slist__(entry));
}

const char *timeline_track_entry_str(struct timeline_track_entry *ctx);
const char *timeline_track_entry_str_r(struct timeline_track_entry *ctx, char *buf);


struct timeline_track {
  /* It's a slist of lists */
  __inherits_from__(struct slist_by_uid);
  /* Should have an unique id */
  unique_id_t uid;
  /* Here's the beginning of the track */
  list_head_t(struct timeline_track_entry) list_track_entries;
  /* The indicators we want to play on that particular track */
  slist_head_t(struct indicator) slist_indicators;
};

static inline int
timeline_track_init(struct timeline_track *ctx, unique_id_t uid)
{
  __slist_by_uid_init__(ctx, uid); /* super() */
  list_head_init(&ctx->list_track_entries);
  slist_head_init(&ctx->slist_indicators);
  return uid;
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
  __inherits_from__(struct slist_by_uid);
  /* Should be easy to find (track_entry->track->uid ?) */
  struct timeline_track_entry *track_entry;
};

static inline int
timeline_slice_entry_init(struct timeline_slice_entry *ctx,
                          unique_id_t track_uid,
			  struct timeline_track_entry *track_entry)
{
  __slist_by_uid_init__(ctx, track_uid); /* super() */
  ctx->track_entry = track_entry;
  return 0;
}

#define timeline_slice_entry_alloc(ctx, track_uid, track_entry)         \
  DEFINE_ALLOC(struct timeline_slice_entry, ctx,                        \
               timeline_slice_entry_init, track_uid, track_entry)

struct timeline_slice {
  __inherits_from__(struct list);
  /* It's a time slice */
  time_info_t time; /* No granularity required ? */
  /* Containing one or more timeline_slices_entries */
  slist_head_t(struct timeline_slice_entry) slist_slice_entries;
};

static inline int
timeline_slice_init(struct timeline_slice *ctx, time_info_t time)
{
  __list_init__(ctx); /* super() */
  ctx->time = time;
  slist_head_init(&ctx->slist_slice_entries);
  return 0;
}

#define timeline_slice_alloc(ctx, time)                                 \
  DEFINE_ALLOC(struct timeline_slice, ctx, timeline_slice_init, time)

struct timeline_track_entry*
timeline_slice_track_entry_by_uid(struct timeline_slice *ctx, unique_id_t uid);

/*
 * The final timeline object
 */

struct timeline {
  list_head_t(struct timeline_slice) by_slice;
  slist_head_t(struct timeline_track) by_track;
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
