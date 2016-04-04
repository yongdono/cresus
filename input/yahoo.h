/*
 * Cresus EVO - yahoo.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef YAHOO_H
#define YAHOO_H

#include <stdio.h>
#include "framework/input.h"

struct yahoo {
  /* Inherits from input */
  struct input parent;
  
  FILE *fp;
};

int yahoo_init(struct yahoo *y, const char *filename);
void yahoo_free(struct yahoo *y);

int yahoo_read(struct input *in, struct candle *candle);

#endif
