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

#include "indicator/macd.h"
#include "indicator/mobile.h"
#include "indicator/rs_mansfield.h"

int main(int argc, char **argv) {

  /*
   * Data
   */

  /* Load / filter PX1 (ref) data */
  /* Load /filter some other data */

  /* 
   * Indicators
   */

  /* Set mobile averages */
  struct mobile ema40, ema14, ema5;
  mobile_init(&ema40, MOBILE_EMA, 40, CANDLE_CLOSE, /* seed */);
  mobile_init(&ema14, MOBILE_EMA, 14, CANDLE_CLOSE, /* seed */);
  mobile_init(&ema5, MOBILE_EMA, 5, CANDLE_CLOSE, /* seed */);

  /* MACD */
  struct macd macd;
  macd_init(&macd, 12, 26, 9, /* seed */);

  /* RS mansfield */
  struct rs_mansfield rsm;
  rs_mansfield_init(&rsm, 14, /* seed */, /* ref */);

  /*
   * Put these indicators in some kinda list
   */

  /*
   * Set loop
   */
  for(;;){
    /* 
     * "Read" all data 
     */
    struct list *ptr;
    struct list_timeline_entry *timeline_entry;
    __list_for_each__(&/* list_timeline_entry */, ptr, timeline_entry){
      struct candle candle;
      struct indicator *indicator_entry;
      __input_read__(/* input */, &__timeline_entry__(candle));
      /*
       * Store that candle somewhere. Store all candles in fact, we need all
       * data before applying any data treatment
       */
      __list_for_each__(&/* list_indicator */, ptr, indicator_entry){
	/* Populate indicators */
	__indicator_feed__(indicator_entry, &__timeline_entry__(candle));
	/*
	 * What about the indicator's results ?
	 */
      }
    }
    
    /*
     * Store that candle in
     */
  }

  
  return 0;
}
