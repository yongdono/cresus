/*
 * Cresus EVO - xtrade.h
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 02/05/2018
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */
#ifndef XTRADE_H
#define XTRADE_H

#include <stdio.h>
#include <json-parser/json.h>

#include "framework/alloc.h"
#include "framework/input.h"

#define xtrade_alloc(ctx, filename, from, to)				\
  DEFINE_ALLOC(struct xtrade, ctx, xtrade_init, filename, from, to)
#define xtrade_free(ctx)			\
  DEFINE_FREE(ctx, xtrade_release)

struct xtrade {
  /* Inherits from input */
  __inherits_from_input__;
  /* internals */
  int i;
  json_char *json;
  json_value *value;
  json_value *data;
};

int xtrade_init(struct xtrade *ctx, const char *filename,
		time_info_t from, time_info_t to);
void xtrade_release(struct xtrade *ctx);

#endif
