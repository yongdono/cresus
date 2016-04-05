/*
 * Cresus EVO - input.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/21/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef __Cresus_EVO__input__
#define __Cresus_EVO__input__

#include "engine/candle.h"
#include "timeline_entry.h"

#define __inherits_from_input__ struct input __parent_input__
#define __input__(x) (x)->__parent_input__
#define __input_self__(x) (x)->__self_input__

#define __input_super__(self, read) input_init(&__input__(self), self, read)
#define __input_free__(self) input_free(&__input__(self))

#define __input_read__(self, candle)			\
  __input__(self)->read(&__input__(self), candle);

/* Typedefs */
struct input;
typedef int (*input_read_ptr)(struct input *in, struct candle *candle);

struct input {
  input_read_ptr read;
  void *__self_input__;
};

int input_init(struct input *in, void *self, input_read_ptr read);
void input_free(struct input *in);

#endif /* defined(__Cresus_EVO__input__) */
