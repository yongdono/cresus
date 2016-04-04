/*
 * Cresus EVO - google.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef GOOGLE_H
#define GOOGLE_H

#include <stdio.h>
#include <sys/types.h>

#include "framework/input.h"

struct google {
  /* Super */
  struct input parent;
  
  FILE *fp;
  struct {
    int interval_s;
    int timezone_m;
    int time;
  } parser;
};

int google_init(struct google *g, const char *filename);
void google_free(struct google *g);

int google_read(struct google *g, struct candle *cdl, size_t n);

#endif
