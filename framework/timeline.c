/*
 * Cresus EVO - timeline.c 
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 04/05/16
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 * 
 */

#include <stdlib.h>
#include "timeline.h"

int timeline_init(struct timeline *t, struct input *in) {

  struct candle *candle;
  list_init(&t->list_candle, NULL);

  for(;;){
    if((candle = malloc(sizeof candle))){
      if(input_read(in, candle) < 0)
	break;

      /* Add it to the list */
      __list_add_tail__(&t->list_candle, candle);
      
    }else
      break;
  } 
  
  return 0;
}

void timeline_free(struct timeline *t) {

  list_free(&t->list_candle);
}

struct candle *timeline_get_candle_by_date(struct timeline *t, int date) {

  struct list *ptr;
  struct candle *candle;
  
  __list_for_each__(&t->list_candle, ptr, candle){
    if(candle->date == date)
      return candle;
  }

  return NULL;
}
