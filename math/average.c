//
//  average.c
//  Cresus EVO
//
//  Created by Joachim Naulet on 20/11/2014.
//  Copyright (c) 2014 Joachim Naulet. All rights reserved.
//

#include <stdlib.h>
#include "average.h"

int average_init(struct average *a, int period, double seed) {
  
  a->index = 1;
  a->count = 1;
  a->period = period;
  
  if((a->pool = malloc(sizeof(*a->pool) * period)))
    return -1;
  
  a->pool[0] = seed;
  return 0;
}

void average_free(struct average *a) {
  
  free(a->pool);
}

double average_update(struct average *a, double value) {
  
  double sum = 0.0;
  
  a->index = (a->index + 1) % a->period; /* Inc */
  a->pool[a->index] = value;
  a->count++;
  
  /* Compute simple average */
  if(a->count >= a->period){
    for(int i = a->period; i--;)
      sum += a->pool[i];
    
    return sum / a->period;
  }
  
  return 0.0;
}
