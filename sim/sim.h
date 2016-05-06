/*
 * Cresus EVO - sim.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef SIM_H
#define SIM_H

#include "sim/position.h"
#include "engine/cluster.h"
#include "framework/slist.h"

struct sim {
  struct cluster *cluster;
  slist_head_t(struct position) slist_position_to_open;
  slist_head_t(struct position) slist_position_opened;
  slist_head_t(struct position) slist_position_to_close;
  slist_head_t(struct position) slist_position_closed;
};

/* types */
typedef int (*sim_feed_ptr)(struct sim*, struct cluster*);

/* API */
int sim_init(struct sim *s, struct cluster *c);
void sim_free(struct sim *s);

int sim_run(struct sim *s, sim_feed_ptr feed);
int sim_open_position(struct sim *s, struct position *p);
int sim_close_position(struct sim *s, struct position *p);

#endif
