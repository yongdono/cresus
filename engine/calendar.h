#ifndef CALENDAR_H
#define CALENDAR_H

#include "framework/types.h"

struct calendar {
  time_info_t time;
  granularity_t g;
  /* Debug */
  char str[256];
};

int calendar_init(struct calendar *c, time_info_t time, granularity_t g);
void calendar_release(struct calendar *c);

int calendar_next(struct calendar *c, time_info_t *time);
const char *calendar_str(struct calendar *c);

#endif
