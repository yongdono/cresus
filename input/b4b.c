/*
 * Cresus EVO - b4b.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 12/30/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <string.h>
#include <stdlib.h>

#include "b4b.h"
#include "engine/candle.h"
#include "framework/verbose.h"

static size_t b4b_prepare_str(struct b4b *ctx, char *buf)
{
  int i;
  char *str = buf;
  
  for(i = 0; buf[i]; i++){
    /* Ignore parenthesis */
    if(buf[i] == '\"')
      continue;
    
    /* Change commas to points */
    if(buf[i] == ',') *str++ = '.';
    *str++ = buf[i];
  }

  return i;
}

static struct candle *b4b_parse_entry(struct b4b *ctx, char *str)
{
  struct candle *c; 
  time_info_t time = 0;
  int year, month, day;
  double open, close, high, low, volume; 
  
  /* Cut string */
  char *stime = strsep(&str, ";");
  char *sclose = strsep(&str, ";");
  char *sopen = strsep(&str, ";");
  char *shi = strsep(&str, ";");
  char *slo = strsep(&str, ";");
  char *svol = strsep(&str, ";"); /* End */
  
  /* Set values */
  sscanf(stime, "%d/%d/%d", &day, &month, &year);
  sscanf(sopen, "%lf", &open);
  sscanf(sclose, "%lf", &close);
  sscanf(shi, "%lf", &high);
  sscanf(slo, "%lf", &low);
  sscanf(svol, "%lf", &volume);

  /* Dummy values for control */
  TIME_SET_SECOND(time, 1);
  TIME_SET_MINUTE(time, 30);
  TIME_SET_HOUR(time, 17);
  
  TIME_SET_DAY(time, day);
  TIME_SET_MONTH(time, month);
  TIME_SET_YEAR(time, year);

  if(candle_alloc(c, time, GRANULARITY_DAY,
		  open, close, high, low, volume))
    return c;

  return NULL;
}

static struct timeline_entry *b4b_read(struct input *in)
{
  struct b4b *ctx = __input_self__(in);
  
  char buf[256];
  struct candle *c;

  while(fgets(buf, sizeof buf, ctx->fp)){
    /* Prepare string */
    b4b_prepare_str(ctx, buf);
    /* Parse entry */
    if((c = b4b_parse_entry(ctx, buf))){
      time_info_t time = __timeline_entry__(c)->time;
      if(TIMECMP(time, __input__(ctx)->from, GRANULARITY_DAY) >= 0 &&
	 TIMECMP(time, __input__(ctx)->to, GRANULARITY_DAY) <= 0){
	/* We got a new candle */
	PR_DBG("%s %.2d/%.2d/%.4d loaded\n", ctx->filename,
	       TIME_GET_MONTH(time), TIME_GET_DAY(time), TIME_GET_YEAR(time));
	return __timeline_entry__(c);
	
      }else
	/* Out of bounds, ignore candle */
	candle_free(c);
    }
  }

  /* End of File */
  return NULL;
}

int b4b_init(struct b4b *ctx, const char *filename,
	     time_info_t from, time_info_t to)
{
  char dummy[256];
  
  /* super */
  __input_super__(ctx, b4b_read, from, to);

  strncpy(ctx->filename, filename, sizeof(ctx->filename));
  if(!(ctx->fp = fopen(ctx->filename, "r"))){
    PR_ERR("unable to open file %s\n", ctx->filename);
    return -1;
  }

  /* Ignore first line */
  fgets(dummy, sizeof(dummy), ctx->fp);
  
  return 0;
}

void b4b_release(struct b4b *ctx)
{
  __input_release__(ctx);
  if(ctx->fp) fclose(ctx->fp);
}
