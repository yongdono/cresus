/*
 * Cresus EVO - development_cluster.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/24/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "input/yahoo.h"

#include "engine/cluster.h"
#include "engine/timeline.h"

#include "indicator/mobile.h"
#include "indicator/rs_mansfield.h"

#define EMA30 1
#define RSM   2

static struct timeline *
timeline_ref_create(const char *filename, const char *name) {
  
  struct yahoo *yahoo;
  struct timeline *timeline;
  
  yahoo_alloc(yahoo, filename, TIME_MIN, TIME_MAX);
  timeline_alloc(timeline, name, __input__(yahoo));

  return timeline;
}

static struct timeline *
timeline_create(const char *filename, const char *name,
		__list_head__(struct timeline_entry) *ref_index) {

  struct yahoo *yahoo;
  struct timeline *timeline;
  
  struct mobile *mobile;
  struct rs_mansfield *rsm;

  /* TODO : Check return values */
  yahoo_alloc(yahoo, filename, TIME_MIN, TIME_MAX);
  timeline_alloc(timeline, name, __input__(yahoo));
  mobile_alloc(mobile, EMA30, MOBILE_EMA, 30, CANDLE_CLOSE);
  rs_mansfield_alloc(rsm, RSM, 14, ref_index);

  return timeline;
}

static void
timeline_destroy(struct timeline *t) {
}

int main(int argc, char **argv) {

  struct cluster cluster;
  struct timeline *t0, *t1, *t2;
  
  cluster_init(&cluster, "my cluster");
  t0 = timeline_ref_create("data/%5EFCHI.yahoo", "^FCHI");
  t1 = timeline_create("data/AF.yahoo", "AF", &t0->list_entry);
  t2 = timeline_create("data/AIR.yahoo", "AIR", &t0->list_entry);

  cluster_add_timeline(&cluster, t0);
  cluster_add_timeline(&cluster, t1);
  cluster_add_timeline(&cluster, t2);

  /* And then ? */
  /* set start time ? */
  while(cluster_step(&cluster)) {
    
  }

  /* TODO : How to sync all that stuff ? */
  
  return 0;
}
