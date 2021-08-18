#ifndef SET_H
#define SET_H

#include <stdio.h>
#include <stdbool.h>
#include "State.h"

// implement Set by Ascending Ordered LinkedList

typedef size_t elem_t;

struct SetElem {
	struct SetElem *prev,*next;
	elem_t elem;
} ;

typedef struct Set {
	struct SetElem *head,*tail;
	unsigned int size;
} Set;

Set* newSet();

struct SetElem* newSetElem(elem_t elem);

void insElem(struct SetElem *prev,struct SetElem *next, struct SetElem *node);

void insert(Set **set,elem_t elem);

bool isEuqal(Set *a,Set *b);

#endif