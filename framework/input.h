/*
 * Cresus EVO - input.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/21/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef __Cresus_EVO__input__
#define __Cresus_EVO__input__

#include <time.h>
#include <limits.h>
#include "timeline_entry.h"

#define __inherits_from_input__ struct input __parent_input__
#define __input_is_superclass__ void *__self_input__
#define __input__(x) (&(x)->__parent_input__)
#define __input_self__(x) (x)->__self_input__
#define __input_self_init__(x, self) __input_self__(x) = self

#define __input_super__(self, read, from, to)	\
  __input_self_init__(__input__(self), self);	\
  input_init(__input__(self), read, from, to)
#define __input_free__(self) input_free(__input__(self))

/* methods */
#define __input_read__(self) input_read(__input__(self))

/* Typedefs */
struct input; /* FIXME : find another way */
typedef struct timeline_entry *(*input_read_ptr)(struct input *in);
/* typedef int (*input_load_ptr)(struct input *in, time_t from, time_t to); */

/* FIXME */
typedef time_t input_time_t;
#define INPUT_TIME_MIN 0
#define INPUT_TIME_MAX INT_MAX

struct input {
  __input_is_superclass__;
  input_read_ptr read;
  time_t from, to;
  /* input_load_ptr load; */
};

static inline int input_init(struct input *in, input_read_ptr read,
			     time_t from, time_t to) {
  in->read = read;
  in->from = from;
  in->to = to;
  return 0;
}

static inline void input_free(struct input *in) {
}

static inline struct timeline_entry *input_read(struct input *in) {
  return in->read(in);
}

/*
static inline int input_load(struct input *in, time_t from, time_t to) {
  return in->load(in, from, to);
}
*/

#endif /* defined(__Cresus_EVO__input__) */
