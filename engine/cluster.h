/*
 * Cresus EVO - cluster.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 20/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef CLUSTER_H
#define CLUSTER_H

#include "framework/slist.h"
#include "engine/timeline.h"

struct cluster {
  /* First, a cluster is a timeline */
  __inherits_from_timeline__;
  /* Second, a cluster's a list of timelines */
  __slist_head__(struct timeline) slist_timeline;
  /* Then, it's an indice computer */
  /* Do we want some indicators on our cluster's data ? */
};

int cluster_init(struct cluster *c, const char *name);
void cluster_free(struct cluster *c);

int cluster_add_timeline(struct cluster *c, struct timeline *t);
int cluster_step(struct cluster *c);

#endif
