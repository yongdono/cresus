/*
 * Cresus EVO - sim.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "sim/sim.h"
#include "sim/statistics.h"
#include "framework/verbose.h"

int sim_init(struct sim *s, struct cluster *c) {

  s->cluster = c;
  /* Init slists */
  list_head_init(&s->list_position_to_open);
  list_head_init(&s->list_position_opened);
  list_head_init(&s->list_position_to_close);
  list_head_init(&s->list_position_closed);

  /* Stats */
  s->factor = 1.0;
  s->nwin = 0;
  s->nloss = 0;

  return 0;
}

void sim_release(struct sim *s) {
  /* Nothing to do */
  list_head_release(&s->list_position_to_open);
  list_head_release(&s->list_position_opened);
  list_head_release(&s->list_position_to_close);
  list_head_release(&s->list_position_closed);
}

static void sim_xfer_positions(struct sim *s,
			       list_head_t(struct position) *dst,
			       list_head_t(struct position) *src,
			       position_action_t position_action) {

  struct list *safe;
  struct position *p;
  
  __list_for_each_safe__(src, p, safe){
    __list_del__(p);
    /* Insert in dst */
    __list_add_tail__(dst, p);
    /* Act on position */
    position_action(p);
  }
}

int sim_run(struct sim *s, sim_feed_ptr feed) {

  while(cluster_step(s->cluster) != -1){
    /* First : check if there are opening positions */
    sim_xfer_positions(s, &s->list_position_opened,
		       &s->list_position_to_open,
		       position_in);
    /* Second : check if there are closing positions */
    sim_xfer_positions(s, &s->list_position_closed,
		       &s->list_position_to_close,
		       position_out);
    /* Third  : feed the sim */
    feed(s, s->cluster);
  }

  return 0;
}

int sim_open_position(struct sim *s, struct timeline *t, 
		      position_t type, int n) {
  
  struct position *p;
  if(position_alloc(p, t, type, n)){
    __list_add__(&s->list_position_to_open, p); /* open could be removed */
    return 0;
  }

  return -1;
}

int sim_close_position(struct sim *s, struct timeline *t) {
  
  int n = 0;
  struct list *safe;
  struct position *p;

  __list_for_each_safe__(&s->list_position_opened, p, safe){
    if(p->t == t){ /* FIXME : use timeline's name ? */
      __list_del__(p); /* FIXME : use safe */
      __list_add_tail__(&s->list_position_to_close, p);
      n++;
    }
  }

  return n;
}

int sim_close_all_positions(struct sim *s) {
  
  sim_xfer_positions(s, &s->list_position_to_close,
		     &s->list_position_opened,
		     position_nop);
  
  return 0;
}

static void sim_stat(struct sim *s, double factor) {

  PR_INFO("stat factor is %.3lf / %.3lf\n", factor, s->factor);

  s->factor *= factor;
  if(factor > 1.0) s->nwin++;
  else s->nloss++;
}

static int sim_report(struct sim *s,
		      list_head_t(struct position) *list) {

  struct position *p;
  struct statistics stat;

  statistics_init(&stat, 0.0);

  __list_for_each__(list, p){
    /* Feed stats for any list here */
    statistics_feed(&stat, position_value(p));
    sim_stat(s, position_factor(p));
  }

  statistics_printf(&stat);
  statistics_release(&stat);

  PR_ERR("SIM FACTOR : %.3lf\n", s->factor);
  PR_ERR("SIM_NWIN : %d\n", s->nwin);
  PR_ERR("SIM_NLOSS : %d\n", s->nloss);

  return 0;
}

int sim_display_report(struct sim *s) {
  
  PR_ERR("Statistics for still opened positions\n");
  sim_report(s, &s->list_position_opened);

  PR_ERR("Statistics for closed positions\n");
  sim_report(s, &s->list_position_closed);

  return 0;
}
