/*
 * Cresus EVO - cluster.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 22/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "cluster.h"

int cluster_init(struct cluster *c, const char *name) {

  /* Super */
  __timeline_super__(c, name);
  __slist_head_init__(&c->slist_timeline);
  
  return 0;
}

void cluster_free(struct cluster *c) {

}

int cluster_add_timeline(struct cluster *c, struct timeline *t) {

  __slist_insert__(&c->slist_timeline, t);
  return 0;
}

int cluster_step(struct cluster *c) {
  
  return 0;
}
