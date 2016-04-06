#ifndef TIMELINE_H
#define TIMELINE_H

#include "framework/input.h"
#include "framework/timeline_entry.h"

typedef time_t granularity_t;
#define GRANULARITY_SEC   1
#define GRANULARITY_MIN   60
#define GRANULARITY_HOUR  3600
#define GRANULARITY_DAY   86400
#define GRANULARITY_WEEK  432000
#define GRANULARITY_MONTH 1728000
#define GRANULARITY_YEAR  20736000

struct timeline {
  struct timeline_entry list_entry;
  struct timeline_entry *cache;
  granularity_t granularity;
};

int timeline_init(struct timeline *t,
		  granularity_t granularity,
		  struct input *in);
void timeline_free(struct timeline *t);

/* TODO : Fill this */

#endif
