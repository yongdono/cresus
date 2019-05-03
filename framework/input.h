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

#include "framework/alloc.h"
#include "framework/time_info.h"

#define input_entry_interface                   \
  double open, close, high, low, volume

/* Input object format */

struct input_entry {
  /* Time */
  time_info_t time;
  time_gr_t gr;
  /* Data */
  double open, close, high, low, volume;
};

static inline int input_entry_init(struct input_entry *ctx,
				   time_info_t time,
				   time_gr_t gr,
				   double open, double close,
				   double high, double low,
				   double volume)
{
  ctx->time = time;
  ctx->gr = gr;
  ctx->open = open;
  ctx->close = close;
  ctx->high = high;
  ctx->low = low;
  ctx->volume = volume;
  return 0;
}

static inline void input_entry_release(struct input_entry *ctx)
{
  /* To be defined */
}

#define input_entry_alloc(ctx, time, gr, open, close, high, low, volume) \
  DEFINE_ALLOC(struct input_entry, ctx, input_entry_init,		\
	       time, gr, open, close, high, low, volume)
#define input_entry_free(ctx)			\
  DEFINE_FREE(ctx, input_entry_release)

#define __input__(x) ((struct input*)(x))

/* Methods */
#define __input_init__(ctx, read) input_init(__input__(ctx), read)
#define __input_release__(ctx) input_release(__input__(ctx))

#define __input_read__(ctx) input_read(__input__(ctx))

/* Typedefs */
struct input; /* FIXME : find another way */

typedef struct input_entry *(*input_read_ptr)(struct input *ctx);

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

static inline struct input_entry *input_read(struct input *ctx)
{
  return ctx->read(ctx);
}

#endif /* defined(__Cresus_EVO__input__) */
