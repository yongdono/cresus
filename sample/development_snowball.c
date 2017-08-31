/*
 * Cresus EVO - development_snowball.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 07/25/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "input/yahoo.h"
#include "engine/timeline.h"
#include "indicator/mobile.h"
#include "indicator/zigzag.h"
#include "framework/verbose.h"

#include <getopt.h>
#include <string.h>

#define EMA    1
#define ZIGZAG 2

#define START_TIME VAL_YEAR(2012) | VAL_MONTH(1) | VAL_DAY(1)

/* Main info */
#define ZIGZAG_THRES 0.05
#define SNOWBALL_MAX 50 // INT_MAX

static int coeff = 1;
static int count = 0;
static double invest = 0.0;
static double capital = 0.0;
static double pending = 0.0;
static zigzag_dir_t __trend__;
static double zz_thres = ZIGZAG_THRES;

/* sim interface */

static int trend_set(zigzag_dir_t trend) {
  
  zigzag_dir_t old = __trend__;
  __trend__ = trend;
  /* Choose what's best here */
  return (__trend__ != old);
}

static int snowball_feed(struct timeline *t,
			 struct timeline_entry *e) {
  
  struct position *p;
  struct indicator_entry *i;
  
  /* TODO : better management of this ? */
  struct candle *c = __timeline_entry_self__(e);
  if((i = candle_find_indicator_entry(c, ZIGZAG))){
    struct zigzag_entry *z = __indicator_entry_self__(i);
    /* PR_WARN("ZIGZAG is %.2f\n", z->value); */
    /* Reset counter when trend reverses */
    if(trend_set(z->dir))
      coeff = 1;
    
    /* Do what you have to do here */
    if(z->dir == ZIGZAG_UP){
      if(count > -SNOWBALL_MAX){
	/* Sell */
	if(count > 0) capital += c->close;// * coeff;
	if(count <= 0) invest += c->close;// * coeff;
	count--;
      }
      
    }else{
      if(count < SNOWBALL_MAX){
	/* Buy */
	if(count >= 0) invest += c->close;// * coeff;
	if(count < 0) capital += c->close;// * coeff;
	count++;
      }
    }
    
    /* Increment */
    coeff++;
  }
  
  PR_INFO("Coeff = %d, count = %d\n", coeff, count);
  pending = abs(count) * c->close;
  return 0;
}

static struct timeline *
timeline_create(const char *filename, const char *name, time_info_t min) {
  
  struct yahoo *yahoo;
  struct zigzag *zigzag;
  struct timeline *timeline;
  
  /* TODO : Check return values */
  yahoo_alloc(yahoo, filename, min, TIME_MAX); /* load everything */
  timeline_alloc(timeline, name, __input__(yahoo));
  /* Indicators alloc */
  zigzag_alloc(zigzag, ZIGZAG, zz_thres, CANDLE_CLOSE);
  /* And insert */
  timeline_add_indicator(timeline, __indicator__(zigzag));
  
  return timeline;
}

int main(int argc, char **argv) {

  int c;
  struct timeline *t;
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

  while(timeline_entry_next(t, &e) != -1){
    /* Step */
    timeline_step(t);
    /* Algo */
    snowball_feed(t, e);
  }

  /* Display stats */
  PR_INFO("Invest = %.2lf Capital = %.2lf (%.2lf : 1)\n" \
	  "Pending = %.2lf (%.2lf : 1) Count = %d\n",
	  invest, capital, capital / invest,
	  pending, (capital + pending) / invest, count);
  
  return 0;
}