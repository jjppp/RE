#include "DFA.h"

DFA *newDFA(unsigned int size) {
	DFA *dfa=(DFA*)malloc(sizeof(DFA));
	dfa->size=size; dfa->state=0;
	
	dfa->is_recv=(bool*)calloc(sizeof(bool),size+1);
	
	dfa->trans=(DFAtransNode*)calloc(sizeof(DFAtransNode),size+1);
	return dfa;
}

int addDFATrans(DFA *dfa,state_t from,state_t to,char_t ch) {
	if (to>dfa->size) {
		puts("Out of Size!");
		return -1;
	}

	dfa->trans[from][ch]=to;
	printf("addDFATrans %d -> %d, ch=%c\n",from,to,ch);
	return 0;
}

int addDFARecv(DFA *dfa,state_t recv) {
	if (recv>dfa->size) {
		puts("Out of Size!");
		return -1;
	}

	dfa->is_recv[recv]=true;
	return 0;
}

state_t moveDFA(DFA *dfa,char_t ch) {
	state_t state=dfa->state;
	if (!dfa->trans[state][ch]) {
		puts("No Such Transfer!");
		return -1;
	}
	return dfa->state=dfa->trans[state][ch];
}

void epClosureSingle(NFA *nfa,StateList *ret_list,state_t state) {
	StateList *succ_list=getNFASucc(nfa,state,0);
	mergeList(ret_list,succ_list);
}

void epClosure(NFA *nfa,StateList *list) {
	StateList *tmp_list=newStateList();
	while (true) {
		int rec_size=list->size;

		NFAState *it=list->head;
		for (;it!=NULL;it=it->next) {
			epClosureSingle(nfa,tmp_list,it->state);
		}

		mergeList(list,tmp_list);


		if (rec_size==list->size) break;
	}
}

DFA *NFAtoDFA(NFA *nfa) {
	DFA *dfa=newDFA(nfa->size*100); // to be improved
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
				if (isEqual(lists[i],*tar_list)) {
					addDFATrans(dfa,top,i,ch);
					flag=true;
				}
			}
			
			if (!flag) {
				lists[++total_list]=*tar_list;
				addDFATrans(dfa,top,total_list,ch);
				stack[++stack_size]=total_list;
			} else free(tar_list);
		}
	}
	for (int i=1;i<=total_list;++i) {
		if (isIn(&lists[i],nfa->end)) {
			addDFARecv(dfa,i);
		}
	}
	dfa->size=total_list;
	free(lists);
	free(stack);

	return dfa;
}

struct edge {
	int y,next;
} *e;

int *ls,*bel,edCnt;

void addEdge(state_t x,state_t y) {
	e[++edCnt]=(struct edge) {y,ls[x]}; ls[x]=edCnt;
}

void color(state_t x,int col) {
	if (bel[x]) return ;
	bel[x]=col;
	for (int i=ls[x];i;i=e[i].next) {
		color(e[i].y,col);
	}
}

DFA *minDFA(DFA *dfa) {
	unsigned int res_dfa_size=2;

	bel=(int*)calloc(sizeof(int),(dfa->size+1));
	
	ls=(int*)calloc(sizeof(int),(dfa->size+1));
	e=(struct edge*)calloc(sizeof(struct edge),(dfa->size+1)*(dfa->size+1));
	edCnt=0;
	
	for (state_t state=1;state<=dfa->size;++state) {
		bel[state]=(dfa->is_recv[state])?1:2;
	}

	while (true) {
		edCnt=0;
		for (state_t state=1;state<=dfa->size;state++)
			ls[state]=0;
		for (state_t sx=1;sx<=dfa->size;++sx) {
			for (state_t sy=sx+1;sy<=dfa->size;++sy) {
				if (bel[sx]!=bel[sy]) continue;
				bool flag=false;
				for (char_t ch=0;ch<CHAR_SIZE;++ch) {
					int tx=dfa->trans[sx][ch],ty=dfa->trans[sy][ch];
					if (!tx||!ty) continue;
					if (bel[dfa->trans[sx][ch]]!=bel[dfa->trans[sy][ch]]) {
						flag=true; break;
					}
				}
				if (!flag) addEdge(sx,sy);
			}
		}
		int col=0;
		for (state_t state=1;state<=dfa->size;state++)
			bel[state]=0;
		for (state_t state=1;state<=dfa->size;state++) {
			if (bel[state]) continue;
			color(state,++col);
		}
		if (col==res_dfa_size) break;
		res_dfa_size=col;
	}
	DFA *res_dfa=newDFA(res_dfa_size);
	for (state_t sx=1;sx<=dfa->size;++sx) {
		if (dfa->is_recv[sx])
			addDFARecv(res_dfa,bel[sx]);
		for (char_t ch=0;ch<CHAR_SIZE;ch++) {
			int from=bel[sx],to=bel[dfa->trans[sx][ch]];
			if (!to) continue;
			addDFATrans(res_dfa,from,to,ch);
		}
	}

	free(bel); free(e); free(ls);
	return res_dfa;
}

void printDFA(DFA *dfa,char *filename) {
	char *buffer=(char*)calloc(sizeof(char),256);
	sprintf(buffer,"%s.gv",filename);
	FILE *file=fopen(buffer,"w");
	fprintf(file,"//Number of Nodes: %d\n",dfa->size);

	int edge_cnt=0;
	for (int state=1;state<=dfa->size;++state)
		for (char_t ch=0;ch<CHAR_SIZE;++ch)
			edge_cnt+=(dfa->trans[state][ch]!=0);

	fprintf(file,"//Number of Edges: %d\n",edge_cnt);
	fprintf(file,"digraph G {\n\tnodesep=1.5;\n\tranksep=0.6;\n\trankdir=LR;\n");
	for (int state=1;state<=dfa->size;++state) {
		// printf("	State: %d, ",state);
		for (char_t ch=0;ch<CHAR_SIZE;++ch) {
			state_t tar_state=dfa->trans[state][ch];
			if (tar_state!=0) {
				fprintf(file,"\t%d -> %d [styel=bold,label=\" %c\"];\n",state,tar_state,ch);
				printf("%d -> %d %c\n",state,tar_state,ch);
			}
		}
	}
	fprintf(file,"\t1:body[shape=doublecircle, width=0.4];\n");
	for (int state=2;state<=dfa->size;++state) {
		if (dfa->is_recv[state]) {
			fprintf(file,"\t%d:body[shape=doublecircle, width=0.4];\n",state);
		} else fprintf(file,"\t%d:body[shape=circle, width=0.4];\n",state);
	}
	fprintf(file,"}\n");
	fclose(file);
	free(buffer);
}