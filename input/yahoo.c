/*
 * Cresus EVO - yahoo.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <string.h>
#include <stdlib.h>

#include "yahoo.h"
#include "engine/candle.h"

static struct timeline_entry *__yahoo_read(struct input *in) {

  struct yahoo *y = __input_self__(in);
  
  y->current_entry = y->current_entry->next;
  if(y->current_entry == &y->list_entry)
    return NULL;
  
  return __list_self__(y->current_entry);
}

static struct timeline_entry *__yahoo_load_entry(struct yahoo *y) {
  
  char buf[256];
  char *str = buf;
  struct candle *ret = NULL;

  struct tm tm;
  int year, month, day;
  double open, close, high, low, volume;
  
  if(!fgets(buf, sizeof buf, y->fp))
    /* End of file */
    return NULL;
  
  /* Cut string */
  char *stime = strsep(&str, ",");
  char *sopen = strsep(&str, ",");
  char *shi = strsep(&str, ",");
  char *slo = strsep(&str, ",");
  char *sclose = strsep(&str, ",");
  char *svol = strsep(&str, ","); /* End */
  
  /* Set values */
  sscanf(stime, "%d-%d-%d", &year, &month, &day);
  sscanf(sopen, "%lf", &open);
  sscanf(sclose, "%lf", &close);
  sscanf(shi, "%lf", &high);
  sscanf(slo, "%lf", &low);
  sscanf(svol, "%lf", &volume);
  
  /* TODO : Set EOD timestamp */
  memset(&tm, 0, sizeof tm);
  tm.tm_min = 30;
  tm.tm_hour = 17;
  tm.tm_mday = day;
  tm.tm_mon = month - 1;
  tm.tm_year = year - 1900;

  /* Create candle (at last !) */
  ret = candle_alloc(mktime(&tm), GRANULARITY_DAY, /* No intraday on yahoo */
		     open, close, high, low, volume);

  if(ret)
    return __timeline_entry__(ret);

  return NULL;
}

static int __yahoo_load(struct yahoo *y) {

  int n = 0;
  char info[256];
  struct timeline_entry *entry;
  
  /* Yahoo is LIFO with first line showing format */
  fgets(info, sizeof info, y->fp);

  while((entry = __yahoo_load_entry(y)) != NULL){
    __list_add__(&y->list_entry, entry);
    n++;
  }

  return n;
}

int yahoo_init(struct yahoo *y, const char *filename) {
  
  /* super */
  __input_super__(y, __yahoo_read);
  __list_head_init__(&y->list_entry);
  
  if(!(y->fp = fopen(filename, "r")))
    return -1;

  /* Load all data */
  __yahoo_load(y);
  
  /* Set pointers */
  y->current_entry = &y->list_entry;
  
  return 0;
}

void yahoo_free(struct yahoo *y) {
  
  __input_free__(y);
  if(y->fp) fclose(y->fp);
}
