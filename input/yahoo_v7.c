/*
 * Cresus EVO - yahoo_v7.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 01/02/2017
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <string.h>
#include <stdlib.h>

#include "yahoo_v7.h"
#include "framework/verbose.h"

static struct input_n3 *
yahoo_v7_parse_n3(struct yahoo_v7 *ctx, char *str)
{
  struct input_n3 *n3;

  time64_t time = 0;
  int year, month, day;
  double open = 0.0, close = 0.0;
  double high = 0.0, low = 0.0;
  double volume = 0.0; 
  
  /* Cut string */
  char *stime = strsep(&str, ",");
  char *sopen = strsep(&str, ",");
  char *shi = strsep(&str, ",");
  char *slo = strsep(&str, ",");
  char *sclose = strsep(&str, ",");
  char *adjclose = strsep(&str, ",");
  char *svol = str; /* End */

  /* Check we got the right format */
  if(!stime || !sopen || !shi || !slo ||
     !sclose || !adjclose || !svol)
    goto err;
  
  /* Set values */
  sscanf(stime, "%d-%d-%d", &year, &month, &day);
  sscanf(sopen, "%lf", &open);
  sscanf(sclose, "%lf", &close);
  sscanf(shi, "%lf", &high);
  sscanf(slo, "%lf", &low);
  sscanf(svol, "%lf", &volume);

  /* Dummy values for control */
  TIME64_SET_SECOND(time, 1);
  TIME64_SET_MINUTE(time, 30);
  TIME64_SET_HOUR(time, 17);
  
  TIME64_SET_DAY(time, day);
  TIME64_SET_MONTH(time, month);
  TIME64_SET_YEAR(time, year);

  if(open != 0.0 && close != 0.0 && high != 0.0 && low != 0.0)
    if(input_n3_alloc(n3, time, open, close, high, low, volume))
      return n3;
  
 err:
  return NULL;
}

static struct input_n3 *yahoo_v7_read(struct input *in)
{
  struct yahoo_v7 *ctx = (void*)in;
  
  char buf[256];
  struct input_n3 *n3;
  
  while(fgets(buf, sizeof buf, ctx->fp)){
    /* Parse n3 */
    if((n3 = yahoo_v7_parse_n3(ctx, buf))){
      PR_DBG("%s %s loaded\n", ctx->filename,
             time64_str_r(n3->time, GR_DAY, buf));
      /* We got a new candle */      
      return n3;
    }
  }
  
  /* End of File */
  return NULL;
}

int yahoo_v7_init(struct yahoo_v7 *ctx, const char *filename)
{
  char dummy[256];
  
  /* init */
  __input_init__(ctx, yahoo_v7_read);

  strncpy(ctx->filename, filename, sizeof(ctx->filename));
  if(!(ctx->fp = fopen(ctx->filename, "r"))){
    PR_ERR("unable to open file %s\n", ctx->filename);
    return -1;
  }

  /* Ignore first line */
  fgets(dummy, sizeof(dummy), ctx->fp);
  
  return 0;
}

void yahoo_v7_release(struct yahoo_v7 *ctx)
{
  __input_release__(ctx);
  if(ctx->fp) fclose(ctx->fp);
}
