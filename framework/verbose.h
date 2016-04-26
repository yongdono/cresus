#ifndef VERBOSE_H
#define VERBOSE_H

#define PR(format, esc, ...)						\
  fprintf(stderr, esc "%s: " format "\x1b[1;0m", __func__, ##__VA_ARGS__)

#define PR_INFO(format, ...) PR(format, "\x1b[1;0m",  ##__VA_ARGS__)
#define PR_WARN(format, ...) PR(format, "\x1b[1;33m", ##__VA_ARGS__)
#define PR_ERR(format, ...)  PR(format, "\x1b[1;31m", ##__VA_ARGS__)

#endif
