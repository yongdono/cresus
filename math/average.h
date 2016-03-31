//
//  average.h
//  Cresus EVO
//
//  Created by Joachim Naulet on 20/11/2014.
//  Copyright (c) 2014 Joachim Naulet. All rights reserved.
//

#ifndef __Cresus_EVO__average__
#define __Cresus_EVO__average__

struct average {
  int count;
  int index;
  int period;
  double *pool;
};

int average_init(struct average *a, int period, double seed);
void average_free(struct average *a);

double average_update(struct average *a, double value);

#endif /* defined(__Cresus_EVO__average__) */
