#include "utils.h"

FILE *Fopen(const char *filename, const char *modes) {
    FILE *fp = fopen(filename, modes);
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }
    return fp;
}

void *Malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        perror("malloc");
        exit(1);
    }
    return ptr;
}
