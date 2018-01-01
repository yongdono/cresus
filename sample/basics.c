/*
 * Cresus EVO - basics.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 07/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 *
 * Development sample code
 * Used to determine if interfaces are coherent for the "user"
 * And maybe to implement some kind of script commands 
 *
 */

#include <stdio.h>
#include <getopt.h>

#include "input/yahoo.h"
#include "engine/engine.h"
#include "indicator/macd.h"
#include "indicator/mobile.h"
#include "indicator/rs_dorsey.h"
#include "indicator/rs_mansfield.h"

#include "framework/verbose.h"

#define EMA40 0
#define EMA14 1
#define EMA5  2
#define MACD  3
#define RSM   4
#define RSD   5

static int feed(struct engine *e,
		struct timeline *t,
		struct timeline_entry *entry)
{
  /* Step by step loop */
  int n = 0;
  struct indicator_entry *ientry;
  struct candle *c = __timeline_entry_self__(entry);
  
  /* Execute */
  PR_INFO("%s - ", candle_str(c));
  /* Then check results */
  __slist_for_each__(&c->slist_indicator, ientry){
    /* Beware, some parsing will be required here to determine who's who */
    printf("%s(%u) ", ientry->indicator->str, ientry->indicator->id);
    switch(ientry->indicator->id){
    case EMA40 :
    case EMA14:
    case EMA5 : {
      struct mobile_entry *mob = __indicator_entry_self__(ientry);
      printf("%.1f ", mob->value);
      goto next;
    }
    case RSM : {
      struct rs_mansfield_entry *rs = __indicator_entry_self__(ientry);
      printf("%.1f ", rs->value);
      goto next;
    }
    case RSD : {
      struct rs_dorsey_entry *rs = __indicator_entry_self__(ientry);
      printf("%.1f ", rs->value);
    } goto next;
    }
  next:
    n++;
  }
  
  printf("- %d\n", n);
  return 0;
}

static struct timeline *timeline_create(const char *filename)
{
  /*
   * Data
   */
  struct yahoo *yahoo;
  struct timeline *timeline;

  if(yahoo_alloc(yahoo, filename, TIME_MIN, TIME_MAX)){
    if(timeline_alloc(timeline, "basics", __input__(yahoo))){
      struct mobile *m;
      /* Add a series of EMAs */
      mobile_alloc(m, EMA40, MOBILE_EMA, 40, CANDLE_CLOSE);
      timeline_add_indicator(timeline, __indicator__(m));
      mobile_alloc(m, EMA14, MOBILE_EMA, 14, CANDLE_CLOSE);
      timeline_add_indicator(timeline, __indicator__(m));
      mobile_alloc(m, EMA5, MOBILE_EMA, 5, CANDLE_CLOSE);
      timeline_add_indicator(timeline, __indicator__(m));
      /* Macd */
      struct macd *macd;
      macd_alloc(macd, MACD, 12, 26, 9);
      timeline_add_indicator(timeline, __indicator__(macd));
      /* RS mansfield */
      struct rs_mansfield *rsm;
      rs_mansfield_alloc(rsm, RSM, 14, &timeline->list_entry);
      timeline_add_indicator(timeline, __indicator__(rsm));
      /* RS Dorsey */
      struct rs_dorsey *rsd;
      rs_dorsey_alloc(rsd, RSD, &timeline->list_entry);
      timeline_add_indicator(timeline, __indicator__(rsd));
      /* Ok */
      return timeline;
    }
  }

  return NULL;
}

int main(int argc, char **argv)
{
  VERBOSE_LEVEL(INFO);
  
  /*
   * Data
   */
  struct timeline *t;
  struct engine engine;

  if(argc != 2){
    fprintf(stdout, "Usage: %s filename.yahoo\n", argv[0]);
    return -1;
  }
  
  if((t = timeline_create(argv[1]))){
    engine_init(&engine, t);
    engine_run(&engine, feed);
  }
  
  /* TODO : Don't forget to release everything */
  engine_release(&engine);
  return 0;
}
