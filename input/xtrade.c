/*
 * Cresus EVO - xtrade.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 02/05/2018
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "xtrade.h"
#include "engine/candle.h"
#include "framework/verbose.h"
#include "framework/time_info.h"

static time_info_t xtrade_time(struct xtrade *ctx,
			       const char *str)
{
  int y, m, d;
  sscanf(str, "%d-%d-%d", &y, &m, &d);
  return TIME_INIT(y, m, d, 0, 0, 0, 0);
}

static struct timeline_entry *xtrade_read(struct input *in)
{
  struct candle *c;
  struct xtrade *ctx = __input_self__(in);
 
  /* Check for EOF at least */
  if(ctx->i <= 0)
    goto err;
  
  json_value *o = ctx->data->u.array.values[ctx->i--];
  char *str = o->u.object.values[0].value->u.string.ptr;
  double open = o->u.object.values[1].value->u.dbl;
  double close = o->u.object.values[2].value->u.dbl;
  double low = o->u.object.values[3].value->u.dbl;
  double high = o->u.object.values[4].value->u.dbl;
  
  time_info_t time = xtrade_time(ctx, str);
  if(input_in_boundaries(in, time, GRANULARITY_DAY))
    /* Filter by time (FIXME ?) */
    if(candle_alloc(c, time, GRANULARITY_DAY,
		    open, close, high, low, 0.0))
      return __timeline_entry__(c);

 err:
  return NULL;
}

int xtrade_init(struct xtrade *ctx, const char *filename,
	      time_info_t from, time_info_t to)
{
  int fd;
  size_t size;
  struct stat stat;

  /* super */
  __input_super__(ctx, xtrade_read, from, to);
  
  /* internals */
  ctx->i = 0;

  if((fd = open(filename, O_RDONLY)) < 0)
    return -1;

  /* Init +exception */
  if(fstat(fd, &stat) < 0)
    goto err;
  /* allocate RAM */
  size = stat.st_size;
  if(!(ctx->json = malloc(size)))
    goto err;
  /* Load entire file to RAM */
  if(read(fd, (void*)ctx->json, size) != size)
    goto err;
  /* json parse */
  if(!(ctx->value = json_parse(ctx->json, size)))
    goto err;
  
  /* Ok */
  ctx->data = ctx->value->u.object.values[2].value;
  ctx->i = ctx->data->u.array.length - 1;
  
  close(fd);
  return 0;
  
 err:
  close(fd);
  PR_ERR("unable to load file %s\n", filename);
  return -1;
}

void xtrade_release(struct xtrade *ctx)
{  
  __input_release__(ctx);
  json_value_free(ctx->value);
  free(ctx->json);
}