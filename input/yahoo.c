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

static int __yahoo_read(struct input *in,
			struct timeline_entry *entry) {
  
  char buf[256];
  char *str = buf;

  struct tm tm;
  int year, month, day;
  double open, close, high, low, volume;
  
  struct yahoo *y = __input_self__(in);
  struct candle *candle = __timeline_entry_self__(entry);

  if(!fgets(buf, sizeof buf, y->fp))
    /* End of file */
    return EOF;
  
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

  /* Setup candle (at last !) */
  candle_init(candle, mktime(&tm), GRANULARITY_DAY, /* No intraday on yahoo */
	      open, close, high, low, volume);
  
  return 1;
}


int yahoo_init(struct yahoo *y, const char *filename) {
  
  /* super */
  __input_super__(y, __yahoo_read);
  
  if(!(y->fp = fopen(filename, "r")))
    return -1;

  return 0;
}

void yahoo_free(struct yahoo *y) {
  
  __input_free__(y);
  if(y->fp) fclose(y->fp);
}
