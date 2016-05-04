/*
 * Cresus EVO - sim.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "sim.h"

int sim_init(struct sim *s, struct cluster *c) {

  s->cluster = c;
  return 0;
}

void sim_free(struct sim *s) {
  /* Nothing to do */
}

int sim_run(struct sim *s, sim_feed_ptr feed) {

  while(cluster_step(s->cluster) != -1)
    feed(s, s->cluster);

  return 0;
}
