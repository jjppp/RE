#ifndef STATE_H
#define STATE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef unsigned int state_t;

typedef unsigned int char_t;

typedef struct NFAState {
	struct NFAState *next,*prev;
	state_t state;
} NFAState;

typedef struct StateList {
	NFAState *head,*tail;
	unsigned int size;
} StateList;

#define CHAR_SIZE 256

NFAState* newNFAState(state_t state);

StateList* newStateList();

void insertState(StateList *list,state_t state);

bool isEqual(StateList *a,StateList *b);

bool isEmpty(StateList *list);

bool isIn(StateList *list,state_t state);

void mergeList(StateList *a,StateList *b);

void printList(StateList *list);

void printGraphvizList(FILE *file,state_t state,char_t ch,StateList *list);

#endif