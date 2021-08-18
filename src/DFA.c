#include "DFA.h"

void newDFA(DFA **dfa,unsigned int size) {
	*dfa=(DFA*)malloc(sizeof(DFA));
	(*dfa)->size=size; (*dfa)->state=0;
	
	(*dfa)->is_recv=(bool*)calloc(sizeof(bool),size+1);
	
	(*dfa)->trans=(DFAtransNode*)calloc(sizeof(DFAtransNode),size+1);
}

int addDFATrans(DFA **dfa,state_t from,state_t to,char_t ch) {
	if (to>=(*dfa)->size) {
		puts("Out of Size!");
		return -1;
	}

	(*dfa)->trans[from][ch]=to;
	printf("addDFATrans %d -> %d, ch=%c\n",from,to,ch);
	return 0;
}

int addDFARecv(DFA **dfa,state_t recv) {
	if (recv>=(*dfa)->size) {
		puts("Out of Size!");
		return -1;
	}

	(*dfa)->is_recv[recv]=true;
	return 0;
}

state_t moveDFA(DFA **dfa,char_t ch) {
	state_t state=(*dfa)->state;
	if (!(*dfa)->trans[state][ch]) {
		puts("No Such Transfer!");
		return -1;
	}
	return (*dfa)->state=(*dfa)->trans[state][ch];
}

void epClosureSingle(NFA *nfa,StateList *ret_list,state_t state) {
	StateList *succ_list=getNFASucc(nfa,state,0);
	mergeList(ret_list,succ_list);
}

void epClosure(NFA *nfa,StateList *list) {
	StateList *tmp_list=newStateList();
	while (true) {
		int rec_size=list->size;
		if (rec_size>10) {
			int ggg=0;
		}

		NFAState *it=list->head;
		for (;it!=NULL;it=it->next) {
			epClosureSingle(nfa,tmp_list,it->state);
		}

		mergeList(list,tmp_list);


		if (rec_size==list->size) break;
	}
}

DFA *NFAtoDFA(NFA *nfa) {
	DFA *dfa=NULL; newDFA(&dfa,nfa->size*100); // to be improved
	StateList *lists=(StateList*)calloc(sizeof(StateList),dfa->size);
	insertState(&lists[1],0);
	epClosure(nfa,&lists[1]);
	int total_list=1;

	int *stack=(int*)calloc(sizeof(int),dfa->size);
	int stack_size=0; stack[++stack_size]=1;
	for (;stack_size;) {
		int top=stack[stack_size--];
		StateList *cur_list=&lists[top];

		for (char_t ch=1;ch<CHAR_SIZE;++ch) {
			StateList *tar_list=newStateList();

			NFAState *it=cur_list->head;
			for (;it!=NULL;it=it->next) {
				StateList *targets=nfa->trans[it->state][ch];
				if (isEmpty(targets)) continue;
				mergeList(tar_list,targets);
			}

			if (isEmpty(tar_list)) {
				free(tar_list);
				continue;
			}

			epClosure(nfa,tar_list);
			bool flag=false;
			for (int i=1;i<=total_list;++i) {
				if (isEqual(&lists[i],tar_list)) {
					addDFATrans(&dfa,top,i,ch);
					flag=true;
				}
			}
			
			if (!flag) {
				lists[++total_list]=*tar_list;
				addDFATrans(&dfa,top,total_list,ch);
				stack[++stack_size]=total_list;
			} else free(tar_list);
		}
	}
	for (int i=1;i<=total_list;++i) {
		if (isIn(&lists[i],nfa->end)) {
			addDFARecv(&dfa,i);
		}
	}
	dfa->size=total_list;
	free(lists);
	free(stack);

	return dfa;
}

void printDFA(DFA *dfa) {
	FILE *file=fopen("DFA.gv","w");
	fprintf(file,"//Number of Nodes: %d\n",dfa->size);
	fprintf(file,"digraph G {\n\tnodesep=1.5;\n\tranksep=0.6;\n\trankdir=LR;\n");
	for (int state=1;state<=dfa->size;++state) {
		// printf("	State: %d, ",state);
		for (int ch=0;ch<CHAR_SIZE;++ch) {
			state_t tar_state=dfa->trans[state][ch];
			if (tar_state!=0) {
				fprintf(file,"\t%d -> %d [styel=bold,label=\" %c\"];\n",state,tar_state,ch);
				printf("%d -> %d %c\n",state,tar_state,ch);
			}
		}
	}
	fprintf(file,"\t1:body[shape=doublecircle, width=0.4];\n",dfa->size);
	for (int state=2;state<=dfa->size;++state) {
		if (dfa->is_recv[state]) {
			fprintf(file,"\t%d:body[shape=doublecircle, width=0.4];\n",state);
		} else fprintf(file,"\t%d:body[shape=circle, width=0.4];\n",state);
	}
	fprintf(file,"}\n");
	fclose(file);
}