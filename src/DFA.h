#ifndef DFA_H
#define DFA_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "State.h"
#include "NFA.h"

typedef state_t DFAtransNode[CHAR_SIZE];

typedef struct DFA {
	DFAtransNode *trans;
	unsigned int size;
	state_t state;
	bool *is_recv;
} DFA;

void newDFA(DFA **dfa,unsigned int size);

int addDFATrans(DFA **dfa,state_t from,state_t to,char_t ch);

int addDFARecv(DFA **dfa,state_t recv);

state_t moveDFA(DFA **dfa,char_t ch);

void printDFA(DFA *dfa);

void epClosureSingle(NFA *nfa,StateList *ret_list,state_t state);

void epClosure(NFA *nfa,StateList *list);

DFA *NFAtoDFA(NFA *nfa);

#endif