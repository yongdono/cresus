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
  } value;
};

#define COMMON_OPT_SET(tuple, type, val)         \
  { (tuple)->set = 1;                            \
    (tuple)->value.type = (val); };

struct common_opt {
  struct common_opt_tuple input_type;
  struct common_opt_tuple fixed_amount;
  struct common_opt_tuple start_time;
  struct common_opt_tuple end_time;
};

int common_opt_init(struct common_opt *ctx);
void common_opt_release(struct common_opt *ctx);

int common_opt_getopt(struct common_opt *ctx, int argc, char **argv);

#endif
