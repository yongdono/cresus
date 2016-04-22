/*
 * Cresus EVO - development.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 07/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 *
 * Development sample code
 * Used to determine if interfaces are coherent for the "user"
 * And maybe to implement some kind of script commands 
 *
 * Not usable / does not compile anyway
 *
 */

#include <stdio.h>
#include <getopt.h>

#include "input/yahoo.h"
#include "engine/timeline.h"
#include "indicator/macd.h"
#include "indicator/mobile.h"
#include "indicator/rs_dorsey.h"
#include "indicator/rs_mansfield.h"

#define EMA40 0
#define EMA14 1
#define EMA5  2
#define MACD  3
#define RSM   4
#define RSD   5

int main(int argc, char **argv) {

  /*
   * Data
   */
  
  /* Load / filter ref data */
  struct yahoo ref;
  yahoo_init(&ref, "data/FCHI.yahoo", TIME_MIN, TIME_MAX);
  
  /*
   * Timeline object
   */
  struct timeline timeline;
  timeline_init(&timeline, "^FCHI", __input__(&ref));

  /* 
   * Indicators
   */
  
  /* Set mobile averages */
  struct mobile ema40, ema14, ema5;
  mobile_init(&ema40, EMA40, MOBILE_EMA, 40, CANDLE_CLOSE);
  mobile_init(&ema14, EMA14, MOBILE_EMA, 14, CANDLE_CLOSE);
  mobile_init(&ema5, EMA5, MOBILE_EMA, 5, CANDLE_CLOSE);
  
  /* MACD */
  struct macd macd;
  macd_init(&macd, MACD, 12, 26, 9);
  
  /* RS mansfield */
  struct rs_mansfield rsm;
  rs_mansfield_init(&rsm, RSM, 14, &timeline.list_entry);

  /* RS Dorsey */
  struct rs_dorsey rsd;
  rs_dorsey_init(&rsd, RSD, &timeline.list_entry);

  /* Add all these indicators */
  timeline_add_indicator(&timeline, __indicator__(&ema40));
  timeline_add_indicator(&timeline, __indicator__(&ema14));
  timeline_add_indicator(&timeline, __indicator__(&ema5));
  timeline_add_indicator(&timeline, __indicator__(&macd));
  timeline_add_indicator(&timeline, __indicator__(&rsm));
  timeline_add_indicator(&timeline, __indicator__(&rsd));
  
  /* Execute all ops on data */
  /* timeline_execute(&timeline, __input__(&ref)); */
  
  /* Step by step loop */
  struct timeline_entry *entry;
  while((entry = timeline_next_entry(&timeline))){
    int n = 0;
    struct indicator_entry *ientry;
    struct candle *c = __timeline_entry_self__(entry);
    /* Execute */
    timeline_step(&timeline);
    printf("%s - ", candle_str(__timeline_entry_self__(entry)));
    /* Then check results */
    __slist_for_each__(&c->slist_indicator, ientry){
      /* Beware, some parsing will be required here to determine who's who */
      printf("%s(%u) ", ientry->indicator->str, ientry->indicator->id);
      switch(ientry->indicator->id){
      case EMA40 :
      case EMA14:
      case EMA5 : {
	struct mobile_indicator_entry *mob =
	  __indicator_entry_self__(ientry);
	
	printf("%.1f ", mob->value);
	goto next;
      }
      case RSM : {
	struct rs_mansfield_indicator_entry *rs =
	  __indicator_entry_self__(ientry);
	
	printf("%.1f ", rs->value);
	goto next;
      }
      case RSD : {
	struct rs_dorsey_indicator_entry *rs =
	  __indicator_entry_self__(ientry);
	
	printf("%.1f ", rs->value);
      } goto next;
      }
    next:
      n++;
    }
    
    printf("- %d\n", n);
  }
  
  return 0;
}
