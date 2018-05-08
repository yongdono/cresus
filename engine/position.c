/*
 * Cresus EVO - position.c
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04.05.2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#include "position.h"
#include "framework/verbose.h"

int position_init(struct position *ctx, double value,
		  position_t type, double n,
		  struct cert *cert)
{
  /* super() */
  __list_super__(ctx);
  
  ctx->n = n;
  ctx->type = type;
  ctx->value = value;

  if(!cert) memset(&ctx->cert, 0, sizeof(ctx->cert));
  else memcpy(&ctx->cert, cert, sizeof(ctx->cert));
  
  return 0;
}

void position_release(struct position *ctx)
{
  __list_release__(ctx);
}

double position_cost(struct position *ctx)
{
  double ret = 0.0;
  if(ctx->type == POSITION_LONG)
    ret = (ctx->value - ctx->cert.funding) * ctx->n;
  else
    ret = (ctx->cert.funding - ctx->value) * ctx->n;
}

double position_value(struct position *ctx, double current)
{
  double ret = 0.0;
  
  if(ctx->type == POSITION_LONG){
    double spread = current - ctx->cert.funding;
    double cost = ctx->value - ctx->cert.funding;
    ret = (spread - cost) * ctx->n;
  }else{
    double spread = ctx->cert.funding - current;
    double cost = ctx->cert.funding - ctx->value;
    ret = (spread - cost) * ctx->n;
  }
  
  return ret;
}
