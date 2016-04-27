/*
 * Cresus EVO - verbose.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 26/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef VERBOSE_H
#define VERBOSE_H

/* FIXME : lib uses this so program must set PROGRAM_USES_VERBOSE anyway.
 * That's not the behavior i want, which is : no inclusion = no verbose
 */

extern int __verbose__;

#define PROGRAM_USES_VERBOSE(level) int __verbose__ = level /* Use as a global */
#define SET_VERBOSE_LEVEL(level) __verbose__ = level

#define DBG  0
#define INFO 1
#define WARN 2
#define ERR  3

#define PR(level, format, esc, ...)					\
  if(level >= __verbose__)						\
    fprintf(stderr, esc "%s: " format "\x1b[1;0m",			\
	    __func__, ##__VA_ARGS__)

#define PR_DBG(format, ...)  PR(DBG, format, "\x1b[1;36m", ##__VA_ARGS__)
#define PR_INFO(format, ...) PR(INFO, format, "\x1b[1;32m", ##__VA_ARGS__)
#define PR_WARN(format, ...) PR(WARN, format, "\x1b[1;33m", ##__VA_ARGS__)
#define PR_ERR(format, ...)  PR(ERR, format, "\x1b[1;31m", ##__VA_ARGS__)

#endif
