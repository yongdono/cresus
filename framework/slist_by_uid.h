#define SLIST_BY_UID_H
#define SLIST_BY_UID_H

#include "framework/types.h"
#include "framework/slist.h"

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

static inline struct slist_by_uid
slist_by_uid_find(struct slist_by_uid *ctx, unique_id_t uid)
{
  struct slist_by_uuid *ptr;
  __slist_for_each__(ctx, ptr){
    if(ptr->uid == uid)
      return ptr;
  }

  return NULL;
}

#endif
