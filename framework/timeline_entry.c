#include "timeline_entry.h"

#define __timeline_entry_time(time, granularity)	\
  (time - (time % granularity))

int timeline_entry_init(struct timeline_entry *e, time_t time,
			granularity_t g) {
  
  __list_super__(e);
  
  e->time = time;
  e->granularity = g;
  
  return 0;
}

void timeline_entry_free(struct timeline_entry *e) {
  
  __list_free__(e);
}

int timeline_entry_timecmp(struct timeline_entry *e, time_t time) {
  
  return (__timeline_entry_time(e->time, e->granularity) -
	  __timeline_entry_time(time, e->granularity));
}

static struct timeline_entry *
timeline_entry_find_forward(struct timeline_entry *e, time_t time) {

  struct list *entry;
  for(entry = __list__(e)->next;
      entry != entry->head;
      entry = entry->next){
    /* Find forward */
    struct timeline_entry *cur = __list_self__(entry);
    if(!timeline_entry_timecmp(cur, time))
      return e;
  }

  return NULL;
}

static struct timeline_entry *
timeline_entry_find_backwards(struct timeline_entry *e, time_t time) {

  struct list *entry;
  for(entry = __list__(e)->prev;
      entry != entry->head;
      entry = entry->prev){
    /* Find forward */
    struct timeline_entry *cur = __list_self__(entry);
    if(!timeline_entry_timecmp(cur, time))
      return e;
  }

  return NULL;
}

struct timeline_entry *
timeline_entry_find(struct timeline_entry *e, time_t time) {
  
  int tm = timeline_entry_timecmp(e, time);
  if(!tm)
    /* time is the same */
    goto out;
  
  if(tm < 0)
    return timeline_entry_find_forward(e, time);
  /* else */
  return timeline_entry_find_backwards(e, time);
  
 out:
  return e;
}

/* Debug functions */
const char *timeline_entry_localtime_str(struct timeline_entry *e,
					 char *buf, size_t len) {
  struct tm tm;
  time_t time = e->time;
  
  strftime(buf, len, "%c", gmtime_r(&time, &tm));
  return buf;
}
