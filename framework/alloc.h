/*
 * Cresus EVO - alloc.h
 *
 * Created by Joachim Naulet <jnaulet@rdinnovation.fr> on 22/04/2016
 * Copyright (c) 2016 Joachim Naulet. All rights reserved.
 *
 */

#ifndef ALLOC_H
#define ALLOC_H

/* TODO : find a way to make an object easily "allocatable" */

#define DEFINE_ALLOC(type, ptr, init, ...)			\
  ((ptr = malloc(sizeof(type))) && !init(ptr, ##__VA_ARGS__))

#endif
