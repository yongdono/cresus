#include <stdio.h>
#include "calendar.h"
#include "framework/verbose.h"

/* TODO : Put this in types.h */
static int day_max[2][12] = {
  { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
  { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
};

int calendar_init(struct calendar *c, time_info_t time, granularity_t g) {

  c->time = time;
  c->g = g;
  
  return 0;
}

void calendar_release(struct calendar *c) {
  /* Nothing to do */
}

static int calendar_is_leap_year(struct calendar *c) {

  int year = TIME_GET_YEAR(c->time);
  return !(year % 400) || ((year % 100) && !(year % 4));
}

static int calendar_day_max(struct calendar *c) {
  
  int month = TIME_GET_MONTH(c->time) - 1;
  int leap = calendar_is_leap_year(c);
  return day_max[leap][month];
}

int calendar_next(struct calendar *c, time_info_t *time) {
  
  /* General increment */
  TIMEADD(c->time, c->g, 1);
  /* Carry management & propagation */
  if(TIME_GET_MSEC(c->time) > MSEC_MAX){
    TIME_SET_MSEC(c->time, 0);
    TIMEADD(c->time, GRANULARITY_SECOND, 1);
  }
  if(TIME_GET_SECOND(c->time) > SECOND_MAX){
    TIME_SET_SECOND(c->time, 0);
    TIMEADD(c->time, GRANULARITY_MINUTE, 1);
  }
  if(TIME_GET_MINUTE(c->time) > MINUTE_MAX){
    TIME_SET_MINUTE(c->time, 0);
    TIMEADD(c->time, GRANULARITY_HOUR, 1);
  }
  if(TIME_GET_HOUR(c->time) > HOUR_MAX){
    TIME_SET_HOUR(c->time, 0);
    TIMEADD(c->time, GRANULARITY_DAY, 1);
  }
  /* This one's more delicate */
  if(TIME_GET_DAY(c->time) > calendar_day_max(c)){
    TIME_SET_DAY(c->time, 1);
    TIMEADD(c->time, GRANULARITY_MONTH, 1);
  }
  if(TIME_GET_MONTH(c->time) > MONTH_MAX){
    TIME_SET_MONTH(c->time, 1);
    TIMEADD(c->time, GRANULARITY_YEAR, 1);
  }

  *time = c->time;
  return 0;
}

const char *calendar_str(struct calendar *c) {

  char *ptr = c->str;
  
  *c->str = 0; /* Zero str */
  if(c->g & YEAR_MASK)
    ptr += sprintf(ptr, "%.4d-", TIME_GET_YEAR(c->time));
  if(c->g & MONTH_MASK)
    ptr += sprintf(ptr, "%.2d-", TIME_GET_MONTH(c->time));
  if(c->g & DAY_MASK)
    ptr += sprintf(ptr, "%.2d", TIME_GET_DAY(c->time));
  if(c->g & HOUR_MASK)
    ptr += sprintf(ptr, " %.02d", TIME_GET_HOUR(c->time));
  if(c->g & MINUTE_MASK)
    ptr += sprintf(ptr, ":%.02d", TIME_GET_MINUTE(c->time));
  if(c->g & SECOND_MASK)
    ptr += sprintf(ptr, ":%.02d", TIME_GET_SECOND(c->time));
  if(c->g & MSEC_MASK)
    ptr += sprintf(ptr, " :%.03d", TIME_GET_MSEC(c->time));
  
  return c->str;
}
