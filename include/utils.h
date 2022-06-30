#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>

FILE *Fopen(const char *filename, const char *modes);

void *Malloc(size_t size);

#endif