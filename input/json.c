/*
 * Cresus EVO - json.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 11/01/2018
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include <string.h>
#include <stdlib.h>

#include "json.h"
#include "engine/candle.h"
#include "framework/verbose.h"

static struct timeline_entry *json_read(struct input *in)
{  
  struct json *ctx = __input_self__(in);
}

int json_init(struct json *ctx, const char *filename,
	      time_info_t from, time_info_t to) {
  
  /* super */
  __input_super__(ctx, json_read, from, to);
  
  if(!(ctx->fp = fopen(filename, "r"))){
    PR_ERR("unable to open file %s\n", filename);
    return -1;
  }
  
  return 0;
}

void json_release(struct json *ctx)
{  
  __input_release__(ctx);
  if(ctx->fp) fclose(ctx->fp);
}
