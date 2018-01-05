/*
 * Cresus EVO - development_snowball.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 07/25/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "input/yahoo.h"
#include "engine/engine.h"
#include "indicator/mobile.h"
#include "framework/verbose.h"

#include <getopt.h>
#include <string.h>

#define EMA    1
#define START_TIME VAL_YEAR(2012) | VAL_MONTH(1) | VAL_DAY(1)

/* Main info */
#define SNOWBALL_EMA 30
#define SNOWBALL_MAX 30

/* Private data */
static int n;
static int last_month = -1;
static int average = SNOWBALL_EMA;
static double share = 0;
static double amount = 0;

#define SIGMA(n) (int)(n * (double)((n + 1)/2.0))

/* sim interface */

static int snowball_feed(struct engine *e,
			 struct timeline *t,
			 struct timeline_entry *entry) {
  
  struct position *p;
  struct indicator_entry *i;
  
  /* TODO : better management of this ? */
  struct candle *c = __timeline_entry_self__(entry);
  /* Every month we recompute the shares */
  if(TIME_GET_MONTH(entry->time) != last_month){
    last_month = TIME_GET_MONTH(entry->time);
    e->amount += 500.0; /* Add 500€ to capital every month */
    amount += 500.0;
    share = e->amount / SIGMA(average);
    PR_INFO("Capital now divided in %d shares of value %.2lf\n",
	    average, SIGMA(average), share);
  }
  /* What do the indicators say ? */
  if((i = candle_find_indicator_entry(c, EMA))){
    struct mobile_entry *m = __indicator_entry_self__(i);
    //PR_WARN("EMA is %.2f going %.2f\n", m->value, m->direction);
    if(m->direction <= 0 && e->amount > share){
      engine_place_order(e, ORDER_BUY, ORDER_BY_AMOUNT, share);
      PR_INFO("Took for %.3lf of positions at %.2lf\n", share, c->close);
      n = n + 1;
    }else
      n = 1;
  }

  PR_INFO("Total portfolio value is %.3lf (%.3lf capital, %.3lf commodities, " \
	  "%.3f money invested)\n",
	  e->amount + (e->npos * c->close), e->amount, (e->npos * c->close),
	  amount);
  
  return 0;
}

static struct timeline *
timeline_create(const char *filename, const char *name, time_info_t min) {
  
  struct yahoo *yahoo;
  struct mobile *mobile;
  struct timeline *timeline;
  
  /* TODO : Check return values */
  yahoo_alloc(yahoo, filename, min, TIME_MAX); /* load everything */
  timeline_alloc(timeline, name, __input__(yahoo));
  /* Indicators alloc */
  mobile_alloc(mobile, EMA, MOBILE_EMA, average, CANDLE_CLOSE);
  /* And insert */
  timeline_add_indicator(timeline, __indicator__(mobile));
  
  return timeline;
}

int main(int argc, char **argv) {

  int c;
  struct timeline *t;
  struct engine engine;
  struct timeline_entry *e;

  /* VERBOSE_LEVEL(WARN); */
  
  while((c = getopt(argc, argv, "va:")) != -1){
    switch(c){
    case 'v' : VERBOSE_LEVEL(DBG); break;
    case 'a' : average = atoi(optarg); break;
    }
  }
  
  /* 01/01/2012 */
  t = timeline_create(argv[optind], "CAC", START_TIME);
  engine_init(&engine, t);

  /* TODO : move this */
  engine.amount = 10000.0;
  engine.npos = 0;
  
  /* Run straight */
  engine_run(&engine, snowball_feed);
  
  /* Display info */
  PR_ERR("Amount available : %.3lf\n", engine.amount);
  PR_ERR("Invested value is now : %.3lf\n", engine.npos);
  
  //timeline_destroy(t);
  engine_release(&engine);
  return 0;
}
