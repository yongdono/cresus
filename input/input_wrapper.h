#ifndef INPUT_WRAPPER_H
#define INPUT_WRAPPER_H

#include <string.h>

#include "input/b4b.h"
#include "input/mdgms.h"
#include "input/xtrade.h"
#include "input/kraken.h"
#include "input/yahoo_v7.h"
#include "input/euronext.h"

static inline struct input *
input_wrapper_create(const char *filename, const char *filetype)
{
  struct input *ret = NULL;
  
  if(!strcmp("b4b", filetype)) b4b_alloc(ret, filename);
  else if(!strcmp("mdgms", filetype)) mdgms_alloc(ret, filename);
  else if(!strcmp("xtrade", filetype)) xtrade_alloc(ret, filename);
  else if(!strcmp("euronext", filetype)) euronext_alloc(ret, filename);
  else if(!strcmp("kraken", filetype)) kraken_alloc(ret, filename);
  /* Default yahoo v7 */
  else yahoo_v7_alloc(ret, filename);
  return ret;
}

#endif
