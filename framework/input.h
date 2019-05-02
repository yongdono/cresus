/*
 * Cresus EVO - input.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 10/21/2014
 * Copyright (c) 2014 Joachim Naulet. All rights reserved.
 *
 */

#ifndef __Cresus_EVO__input__
#define __Cresus_EVO__input__

#include <limits.h>

#define __input__(x) ((struct input*)(x))

/* Methods */
#define __input_init__(ctx, read) input_init(__input__(ctx), read)
#define __input_release__(ctx) input_release(__input__(ctx))

#define __input_read__(ctx) input_read(__input__(ctx))

/* Typedefs */
struct input; /* FIXME : find another way */

#include "timeline_entry.h"

typedef struct timeline_entry *(*input_read_ptr)(struct input *ctx);

struct input {
  input_read_ptr read;
};

static inline int input_init(struct input *ctx, input_read_ptr read)
{
  ctx->read = read;
  return 0;
}

static inline void input_release(struct input *ctx)
{
}

static inline struct timeline_entry *input_read(struct input *ctx)
{
  return ctx->read(ctx);
}

#endif /* defined(__Cresus_EVO__input__) */
