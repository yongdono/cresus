/*
 * Cresus EVO - input.c 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/20/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#include "input.h"

int input_init(struct input *in, void *self, input_read_ptr read) {
  
  in->read = read;
  in->__self_input__ = self;
  return 0;
}

void input_free(struct input *in) {
}
