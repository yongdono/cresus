/*
 * Cresus EVO - list.h 
 * 
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef LIST_H
#define LIST_H

#include <stdio.h>

/* Superclass */

#define __inherits_from_list__ struct list __parent_list__
#define __list_is_superclass__ void *__self_list__
#define __list__(x) (&(x)->__parent_list__)
#define __list_self__(x) ((x)->__self_list__)
#define __list_self_init__(x, self) (x)->__self_list__ = self

#define __list_super__(self)			\
  __list_self_init__(__list__(self), self);	\
  list_init(__list__(self));
#define __list_release__(self) list_release(__list__(self));

#define __list_add__(list, entry)		\
  list_add((list), __list__(entry))
#define __list_add_tail__(list, entry)		\
  list_add_tail((list), __list__(entry))
#define __list_del__(entry)			\
  list_del(__list__(entry))
/* Iteration */
#define __list_for_each__(head, self)			\
  for(struct list *ptr = (head)->next;			\
      ptr != (head) && (self = __list_self__(ptr));	\
      ptr = ptr->next)
#define __list_for_each_prev__(head, self)		\
  for(struct list *ptr = (head)->prev;			\
      ptr != (head) && (self = __list_self__(ptr));	\
      ptr = ptr->prev)
/* Relative functions */
#define __list_relative__(entry, n)			\
  __list_self__(list_relative(__list__(entry), n))

/* Basic list object */
  
#define list_head_t(type) struct list /* Type is purely indicative */
#define list_head_init(x) list_init(x)
#define list_head_release(x) list_release(x)

struct list {
  __list_is_superclass__;
  struct list *head, *prev, *next;
};

static inline int list_init(struct list *l) {
  l->head = l;
  l->next = l;
  l->prev = l;
  return 0;
}

static inline void list_release(struct list *l) {
  l->head = NULL;
  l->next = NULL;
  l->prev = NULL;
}

static inline void list_add(struct list *l, struct list *new) {
  new->prev = l->next->prev;
  new->next = l->next;
  l->next->prev = new;
  l->next = new;
  new->head = l;
}

static inline void list_add_tail(struct list *l, struct list *entry) {
  list_add(l->prev, entry);
  /* Correct head */
  entry->head = l;
}

static inline void list_del(struct list *l) {
  l->next->prev = l->prev;
  l->prev->next = l->next;
  l->head = l;
}

/* #define list_is_head(head, ptr) ((ptr) == (head)) */
#define list_is_head(ptr) ((ptr) == (ptr)->head)

/* Relative functions */

static inline struct list *list_prev_n(struct list *l, int n) {
  for(int i = n; !list_is_head(l) && i--;) l = l->prev;
  return l;
}

static inline struct list *list_next_n(struct list *l, int n) {
  for(int i = n; !list_is_head(l) && i--;) l = l->next;
  return l;
}

#define list_relative(list, n)			\
  (((n) < 0) ? list_prev_n((list), -(n)) :	\
   list_next_n((list), (n)))

#endif
