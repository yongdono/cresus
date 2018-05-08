/*
 * Cresus EVO - position.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04.05.2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef POSITION_H
#define POSITION_H

#include "engine/cert.h"
#include "framework/list.h"
#include "framework/alloc.h"

typedef enum {
  POSITION_LONG,
  POSITION_SHORT
} position_t;

#define position_alloc(p, timeline, value, type, n, cert)		\
  DEFINE_ALLOC(struct position, p, position_init,			\
	       value, type, n, (cert))
#define position_free(p)			\
  DEFINE_FREE(p, position_release)

struct position {
  /* slistable */
  __inherits_from_list__;
  /* Internal data */
  double n; /* How much ? */
  position_t type; /* Why ? */
  double value; /* Unit cost */
  /* Certificates/Turbos */
  struct cert cert;
};

int position_init(struct position *ctx, double value,
		  position_t type, double n, struct cert *cert);
void position_release(struct position *ctx);

double position_cost(struct position *ctx);
double position_value(struct position *ctx, double current);

#endif
