/*
 * Cresus EVO - input.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/21/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef __Cresus_EVO__input__
#define __Cresus_EVO__input__

#include "timeline_entry.h"

#define __inherits_from_input__ struct input __parent_input__
#define __input_is_superclass__ void *__self_input__
#define __input__(x) (&(x)->__parent_input__)
#define __input_self__(x) (x)->__self_input__
#define __input_self_init__(x, self) __input_self__(x) = self

#define __input_super__(self, read)		\
  __input_self_init__(__input__(self), self);	\
  input_init(__input__(self),read)
#define __input_free__(self) input_free(__input__(self))

/* methods */
#define __input_read__(self, entry)				\
  input_read(__input__(self), __timeline_entry__(entry))

/* Typedefs */
struct input; /* FIXME : find another way */
typedef int (*input_read_ptr)(struct input *in,
			      struct timeline_entry *entry);

struct input {
  __input_is_superclass__;
  input_read_ptr read;
};

static inline int input_init(struct input *in, input_read_ptr read) {
  in->read = read;
  return 0;
}

static inline void input_free(struct input *in) {}

static inline int input_read(struct input *in,
			     struct timeline_entry *entry) {
  in->read(in, entry);
}

#endif /* defined(__Cresus_EVO__input__) */
