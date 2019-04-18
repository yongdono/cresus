/*
 * Cresus EVO - common_opt.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 18/04/2019
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef COMMON_OPT_H
#define COMMON_OPT_H

#include "time_info.h"

struct common_opt_tuple {
  int set;
  union {
    int i;
    char *s;
    double d;
    time_info_t t;
  };
};

#define COMMON_OPT_SET(tuple, type, val)         \
  { (tuple)->set = 1;                            \
    (tuple)->type = (val); };

#define COMMON_OPTSTRING "o:F:S:E:"
/*
 * -o file type
 * -F fixed amount
 * -S start time (format : YYYY-MM-DD)
 * -E end time (format : YYYY-MM-DD)
 */

struct common_opt {
  /* Internals */
  char optstring[256];
  /* Data */
  struct common_opt_tuple input_type;
  struct common_opt_tuple fixed_amount;
  struct common_opt_tuple start_time;
  struct common_opt_tuple end_time;
};

int common_opt_init(struct common_opt *ctx, const char *optstring);
void common_opt_release(struct common_opt *ctx);

int common_opt_getopt(struct common_opt *ctx, int argc, char **argv);

#endif
