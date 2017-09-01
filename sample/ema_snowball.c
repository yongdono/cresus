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
static double capital = 30000.0;
static int average = SNOWBALL_EMA;

static int sigma(int n)
{
  int ret = 0;
  for(int i = 0; i <= n; i++)
    ret += i;

  return ret;
}

/* sim interface */

static int snowball_feed(struct engine *e,
			 struct timeline *t,
			 struct timeline_entry *entry) {
  
  struct position *p;
  struct indicator_entry *i;

  int s = sigma(average);
  PR_ERR("Sigma %d = %d, capital shares are %.2f\n", average, s, capital / s);
  
  /* TODO : better management of this ? */
  struct candle *c = __timeline_entry_self__(entry);
  if((i = candle_find_indicator_entry(c, EMA))){
    struct mobile_entry *m = __indicator_entry_self__(i);
    PR_WARN("EMA is %.2f going %.2f\n", m->value, m->direction);
    if(m->direction <= 0){
      engine_open_position(e, t, POSITION_LONG, n);
      PR_INFO("Took %d positions at %.2lf\n", n, c->close);
      n = n + 1;
    }else
      n = 1;
  }
  
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
  
  /* Run straight */
  engine_run(&engine, snowball_feed);
  
  /* Display info */
  engine_close_all_positions(&engine);
  
  //timeline_destroy(t);
  engine_release(&engine);
  return 0;
}
