/*
 * Cresus EVO - common_opt.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 18.04.2019
 * Copyright (c) 2018 Joachim Naulet. All rights reserved.
 *
 */

#include <string.h>
#include <getopt.h>
#include <stdlib.h>

#include "verbose.h"
#include "common_opt.h"

int common_opt_init(struct common_opt *ctx)
{
  memset(ctx, 0, sizeof(*ctx));
  return 0;
}

void common_opt_release(struct common_opt *ctx)
{
}

static time_info_t common_opt_time_info(struct common_opt *ctx, char *str)
{
  /* Time info format : YYYY-MM-DD (date +%Y-%M-%d) */
  char *Y = strsep(&str, "-");
  char *M = strsep(&str, "-");
  char *d = strsep(&str, "-"); /* End */
  return TIME_INIT(atoi(Y), atoi(M), atoi(d), 0, 0, 0, 0);
}

int common_opt_getopt(struct common_opt *ctx, int argc, char **argv)
{
  int c;

  while((c = getopt(argc, argv, "o:F:S:E:")) != -1){
    switch(c){
    case 'o':
      COMMON_OPT_SET(&ctx->input_type, s, optarg);
      break;
      
    case 'F':
      COMMON_OPT_SET(&ctx->fixed_amount, i, atoi(optarg));
      break;
      
    case 'S':
      COMMON_OPT_SET(&ctx->start_time, t,
                     common_opt_time_info(ctx, optarg));
      break;
      
    case 'E':
      COMMON_OPT_SET(&ctx->end_time, t,
                     common_opt_time_info(ctx, optarg));
      break;
      
    default:
      PR_ERR("Unknown option %c\n", c);
      return -1;
    }
  }
  
  return 0;
}
