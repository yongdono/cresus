/*
 * Cresus EVO - development_sim.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 05/04/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "sim/sim.h"
#include "input/yahoo.h"
#include "engine/cluster.h"
#include "engine/timeline.h"
#include "indicator/mobile.h"
#include "indicator/roc.h"
#include "indicator/jtrend.h"
#include "framework/verbose.h"

#include <string.h>

#define EMA    1
#define ROC    2
#define JTREND 3

/* Main info */
#define PERIOD 24
#define AVERAGE 9

typedef enum {
  TREND_NONE,
  TREND_UP,
  TREND_DOWN,
} trend_t;

/* Global */
static trend_t __trend__;

static int trend_set(trend_t trend) {
  
  trend_t old = __trend__;
  __trend__ = trend;
  /* Choose what's best here */
  return !(__trend__ == old);
}


static struct timeline *
timeline_create(const char *filename, const char *name, time_info_t min,
		list_head_t(struct timeline_entry) *ref_index) {

  struct yahoo *yahoo;
  struct timeline *timeline;
  
  struct mobile *mobile;
  struct jtrend *jtrend;

  /* TODO : Check return values */
  yahoo_alloc(yahoo, filename, min, TIME_MAX); /* load everything */
  timeline_alloc(timeline, name, __input__(yahoo));
  /* Indicators alloc */
  mobile_alloc(mobile, EMA, MOBILE_EMA, 30, CANDLE_CLOSE);
  jtrend_alloc(jtrend, JTREND, PERIOD, AVERAGE, ref_index);
  /* And insert */
  timeline_add_indicator(timeline, __indicator__(mobile));
  timeline_add_indicator(timeline, __indicator__(jtrend));
  
  return timeline;
}

static void timeline_destroy(struct timeline *t) {
}

static void timeline_display_info(struct timeline *t) {

  char buf[256]; /* debug */
  
  /* FIXME : change interface */
  struct timeline_entry *entry;
  if(timeline_entry_current(t, &entry) != -1){
    struct indicator_entry *ientry;
    struct candle *candle = __timeline_entry_self__(entry);
    /* Indicators management */
    /* This interface is not easy to use. Find something better */
    candle_indicator_for_each(candle, ientry) {
      switch(ientry->iid){
      case EMA : PR_WARN("%s EMA is %.2f\n", t->name,
			 ((struct mobile_entry*)
			  __indicator_entry_self__(ientry))->value);
	break;
	
      case JTREND : {
	struct jtrend_entry *e = __indicator_entry_self__(ientry);
	PR_WARN("%s JTREND is %.2f, %.2f\n", t->name,
		e->value, e->ref_value);
	
	if(e->value > 0 && e->ref_value > 0){
	  PR_ERR("Taking LONG position on %s at %s\n",
		 t->name, candle_str(candle, buf));
	}
	
	if(e->value < 0 && e->ref_value < 0){
	  PR_ERR("Taking SHORT position on %s at %s\n",
		 t->name, candle_str(candle, buf));
	}}
	break;
      }
    }
  }
}

/* sim interface */

static int sim_feed(struct sim *s, struct cluster *c) {

  char buf[256];
  int status = 0;
  
  struct timeline *t;
  struct timeline_entry *entry;
  struct indicator_entry *ientry;
  
  /* TODO : better management of this ? */
  if(timeline_entry_current(__timeline__(c), &entry) != -1){
    struct candle *candle = __timeline_entry_self__(entry);
    if((ientry = candle_find_indicator_entry(candle, ROC))){
      struct roc_entry *rentry = __indicator_entry_self__(ientry);
      PR_WARN("%s ROC is %.2f\n", __timeline__(c)->name, rentry->value);
      /* Manage cluster's status here */
      if(trend_set(rentry->value > 0 ? TREND_UP : TREND_DOWN)){
	/* We got a change here */
	PR_INFO("General trend switched to %s\n",
		__trend__ == TREND_UP ? "up" : "down");
	/* Close all positions */
	sim_close_all_positions(s);
      }
    }
  }
  
  __slist_for_each__(&c->slist_timeline, t){
    /* TODO : this is facultative */
    //timeline_display_info(t);
    /* This is mandatory */
    if(timeline_entry_current(t, &entry) != -1){
      struct candle *candle = __timeline_entry_self__(entry);
      if((ientry = candle_find_indicator_entry(candle, JTREND))){
	struct jtrend_entry *jentry = __indicator_entry_self__(ientry);
	PR_WARN("%s JTREND is %.2f, %.2f\n", t->name,
		jentry->value, jentry->ref_value);

	if(jentry->value > 0 && __trend__ == TREND_UP){
	  PR_ERR("Taking LONG position on %s at %s\n",
		 t->name, candle_str(candle, buf));

	  sim_open_position(s, t, POSITION_LONG, 1);
	}

	if(jentry->value < 0 && __trend__ == TREND_DOWN){
	  PR_ERR("Taking SHORT position on %s at %s\n",
		 t->name, candle_str(candle, buf));

	  sim_open_position(s, t, POSITION_SHORT, 1);
	}
      }
    }
  }
  
  return 0;
}

/* more final functions */

static void add_timeline_to_cluster(struct cluster *c, const char *path,
				    const char *name, time_info_t time) {

  struct timeline *sub;
  sub = timeline_create(path, name, time, &__timeline__(c)->list_entry);
  cluster_add_timeline(c, sub);
}

int main(int argc, char **argv) {

  struct sim sim;
  struct roc roc;
  struct cluster cluster;
  
  if(argc > 1 && !strcmp(argv[1], "-v"))
    VERBOSE_LEVEL(DBG);
  
  /* 01/01/2000 */
  time_info_t time = TIME_INIT(2000, 1, 1, 0, 0, 0, 0);
  cluster_init(&cluster, "my cluster", time, TIME_MAX);
  /* Init general roc indicator */
  roc_init(&roc, ROC, PERIOD, AVERAGE);
  timeline_add_indicator(__timeline__(&cluster), __indicator__(&roc));
  
  /* Sub-timelines */
  add_timeline_to_cluster(&cluster, "data/AF.yahoo", "AF", time);
  add_timeline_to_cluster(&cluster, "data/AIR.yahoo", "AIR", time);
  
  /* Now create sim */
  sim_init(&sim, &cluster);
  sim_run(&sim, sim_feed);
  sim_display_report(&sim);
  
  return 0;
}
