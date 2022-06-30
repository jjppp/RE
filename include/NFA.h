#ifndef NFA_H
#define NFA_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "State.h"

typedef StateList *NFAtransNode[CHAR_SIZE];

typedef struct NFA {
	NFAtransNode *trans;
	state_t start,end;
	unsigned int size;
	bool *is_recv,is_start;
} NFA;

NFA *newNFA(unsigned int size);

int addNFATrans(NFA *nfa,state_t from,state_t to,char_t ch);

int addNFARecv(NFA *nfa,state_t recv);

StateList *getNFASucc(NFA *nfa,state_t state,char_t ch);

void printNFA(NFA *nfa);

#endif
