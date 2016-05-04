/*
 * Cresus EVO - sim.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef SIM_H
#define SIM_H

#include "engine/cluster.h"

struct sim {
  struct cluster *cluster;
};

/* types */
typedef int (*sim_feed_ptr)(struct sim*, struct cluster*);

/* API */
int sim_init(struct sim *s, struct cluster *c);
void sim_free(struct sim *s);

int sim_run(struct sim *s, sim_feed_ptr feed);

#endif
