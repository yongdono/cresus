#ifndef SLIST_BY_UID_H
#define SLIST_BY_UID_H

#include "framework/types.h"
#include "framework/slist.h"
#include "framework/alloc.h"

#define __slist_by_uid__(x) ((struct slist_by_uid*)(x))

struct slist_by_uid {
  __inherits_from__(struct slist);
  unique_id_t uid;
};

static inline int
slist_by_uid_init(struct slist_by_uid *ctx, unique_id_t uid)
{
  __slist_init__(ctx); /* super() */
  ctx->uid = uid;
  return 0;
}

#define __slist_by_uid_init__(ctx, uid)         \
  slist_by_uid_init(__slist_by_uid__(ctx), uid)

static inline struct slist_by_uid *
slist_by_uid_find(struct slist_by_uid *ctx, unique_id_t uid)
{
  struct slist_by_uid *ptr;
  __slist_for_each__(ctx, ptr){
    if(ptr->uid == uid)
      return ptr;
  }

  return NULL;
}

#define __slist_by_uid_find__(ctx, uid)         \
  slist_by_uid_find(__slist_by_uid__(ctx), uid)

#endif
