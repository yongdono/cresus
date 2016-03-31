#ifndef SWING_H
#define SWING_H

#include "framework/candle.h"

#define SWING_MAX 3

typedef enum {
  SWING_NONE,
  SWING_TOP,
  SWING_BOTTOM
} swing_t;

struct swing {
  int count;
  int index;

  swing_t type;
  struct candle pool[SWING_MAX];
};

int swing_init(struct swing *s, struct candle *cdl);
void swing_free(struct swing *s);

struct candle *swing_feed(struct swing *s, struct candle *cdl);
swing_t swing_type_get(struct swing *s);

#endif
