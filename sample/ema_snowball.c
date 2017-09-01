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

/* sim interface */

static int snowball_feed(struct timeline *t,
			 struct timeline_entry *e) {
  
  struct position *p;
  struct indicator_entry *i;
  
  /* TODO : better management of this ? */
  struct candle *c = __timeline_entry_self__(e);
  if((i = candle_find_indicator_entry(c, EMA))){
    struct mobile_entry *m = __indicator_entry_self__(i);
    PR_WARN("EMA is %.2f going %.2f\n", m->value, m->direction);
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
  mobile_alloc(mobile, EMA, MOBILE_EMA, SNOWBALL_EMA, CANDLE_CLOSE);
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
  
  while((c = getopt(argc, argv, "vp:a:")) != -1){
    switch(c){
    case 'v' : VERBOSE_LEVEL(DBG); break;
      /* case 'p' : period = atoi(optarg); break;
	 case 'a' : average = atoi(optarg); break; */
    }
  }
  
  /* 01/01/2012 */
  t = timeline_create("data/%5EFCHI.yahoo", "CAC", START_TIME);
  engine_init(&engine, t);
  
  /* Run straight */
  engine_run(&engine, snowball_feed);
  
  /* Display info */
  
  return 0;
}
