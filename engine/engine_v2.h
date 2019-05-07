/*
 * Cresus CommonTL - engine_v2.h
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 06/05/2019
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef ENGINE_V2_H
#define ENGINE_V2_H

#include "framework/timeline.h"
#include "framework/time64.h"

#include "engine/common_opt.h"

struct engine_v2 {
  struct timeline *timeline;
  /* Positions filter */
  time64_t start_time;
  time64_t end_time;
  /* Stats */
  double transaction_fee; /* Cost per transaction */
  /* CSV graph output */
  int csv_output;
};

/* External pointer to plugin */
typedef void (*engine_v2_feed_slice_ptr)(struct engine_v2*, struct timeline_slice*);
typedef void (*engine_v2_feed_track_n3_ptr)(struct engine_v2*, struct timeline_slice*, struct timeline_track_n3*);
typedef void (*engine_v2_feed_indicator_n3_ptr)(struct engine_v2*, struct timeline_track_n3*, struct indicator_n3*);
typedef void (*engine_v2_post_slice_ptr)(struct engine_v2*, struct timeline_slice*);

struct engine_v2_interface {
  engine_v2_feed_slice_ptr feed_slice;
  engine_v2_feed_track_n3_ptr feed_track_n3;
  engine_v2_feed_indicator_n3_ptr feed_indicator_n3;
  engine_v2_post_slice_ptr post_slice;
};

int engine_v2_init(struct engine_v2 *ctx, struct timeline *t);
void engine_v2_release(struct engine_v2 *ctx);

int engine_v2_set_common_opt(struct engine_v2 *ctx, struct common_opt *opt);
void engine_v2_run(struct engine_v2 *ctx, struct engine_v2_interface *i);

#endif
