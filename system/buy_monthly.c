/*
 * Cresus EVO - buy_monthly.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 25/02/2018
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
#include <math.h>

#include "input/inwrap.h"
#include "engine/engine.h"
#include "framework/verbose.h"

static int amount = 250;
static int occurrence = 1;
time_info_t year_min = TIME_ZERO();

/* Stats */
static int quiet = 0;

static int feed(struct engine *e,
		struct timeline *t,
		struct timeline_entry *entry)
{
  /* Step by step loop */
  static int last_month = -1;
  time_info_t time = VAL_YEAR(year_min);
  struct candle *c = __timeline_entry_self__(entry);
  
  /* Execute */
  int month = TIME_GET_MONTH(entry->time);
  if(month != last_month && !(month % occurrence))
    engine_set_order(e, BUY, amount, CASH, NULL);
  
  last_month = month;
  return 0;
}

static struct timeline *timeline_create(const char *filename,
					const char *type)
{
  /*
   * Data
   */
  struct inwrap *inwrap;
  struct timeline *timeline;
  inwrap_t t = inwrap_t_from_str(type);
  
  if(inwrap_alloc(inwrap, filename, t, TIME_MIN, TIME_MAX)){
    if(timeline_alloc(timeline, "buy_monthly", __input__(inwrap))){
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
  int c;
  char *filename, *type;
  
  struct timeline *t;
  struct engine engine;

  if(argc < 2){
    fprintf(stdout, "Usage: %s -o type filename\n", argv[0]);
    return -1;
  }

  while((c = getopt(argc, argv, "o:m:n:F:q")) != -1){
    switch(c){
    case 'o': type = optarg; break;
    case 'm': occurrence = atoi(optarg); break;
    case 'n': year_min = VAL_YEAR(atoi(optarg)); break;
    case 'F': amount = atoi(optarg); break;
    case 'q': quiet = 1; break;
    default:
      PR_ERR("Unknown option %c\n", c);
      return -1;
    }
  }

  /* Filename is only real param */
  filename = argv[optind];
  
  if((t = timeline_create(filename, type))){
    engine_init(&engine, t);
    /* Options */
    engine_set_transaction_fee(&engine, 2.50);
    engine_set_quiet(&engine, quiet);
    engine_set_filter(&engine, year_min);

    /* Run */
    engine_run(&engine, feed);
    
    /* Print some info */
    engine_display_stats(&engine);
    
    /* TODO : Don't forget to release everything */
    engine_release(&engine);
  }
  
  return 0;
}
