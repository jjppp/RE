#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <assert.h>

#define DEBUG

#ifdef DEBUG
#define debug(...) \
  do { \
    fprintf(stderr, "\033[31;1;4mLog\033[0m:" __VA_ARGS__); \
  } while (0);

#define Assert(exp, ...) \
  do { if (!exp) { \
    fprintf(stderr, __VA_ARGS__); \
    exit(1); \
  } } while (0);
#else
#define debug(...)
#define Assert(exp, ...)
#endif

#endif