/* A Regular Expression Engine
* Valid Characters:
* (,),*,|
* @ stands for \epsilon
*
*
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "State.h"
#include "Expr.h"
#include "DFA.h"
#include "NFA.h"

#define DFA_SIZE 200005
#define STACK_SIZE 200005

int left_brac[STACK_SIZE],prior[CHAR_SIZE],char_stk_top,expr_stk_top;

char char_stk[STACK_SIZE];
Expr *expr_stk[STACK_SIZE];

int preProc(char *str,int len) {
	char *buf=(char*) calloc(sizeof(char),STACK_SIZE);
	strcpy(buf,str);
	int slen=1; str[0]=buf[0];
	for (int i=1,_=len;i<_;++i) {
		if ((buf[i-1]!='|'&&buf[i-1]!='(')&&(buf[i]!='|'&&buf[i]!='*'&&buf[i]!=')')) {
			str[slen++]='+';
		}
		str[slen++]=buf[i];
	}
	str[slen]=0;
	printf("%s\n",str);
	free(buf); buf=NULL;
	return slen;
}

int proc(char *str,char *ptr,int len) {
	prior['|']=1; prior['+']=2; prior['*']=3;
	len=preProc(str,len);
	int plen=0;
	for (int i=0;i<len;++i) {
		switch (str[i]) {
			case '(':
				char_stk[++char_stk_top]=str[i];
				break;
			case ')':
				while (char_stk[char_stk_top]!='(') {
					ptr[plen++]=char_stk[char_stk_top--];
				} char_stk_top--;
				break;
			case '|':case'*':case '+':
				while (char_stk_top&&prior[(int)char_stk[char_stk_top]]>=prior[(int)str[i]]) {
					ptr[plen++]=char_stk[char_stk_top--];
				} char_stk[++char_stk_top]=str[i];
				break;
			default: ptr[plen++]=str[i];
		}
	}
	while (char_stk_top) ptr[plen++]=char_stk[char_stk_top--];
	ptr[plen]=0;
	printf("%s\n",ptr);
	return plen;
}

Expr* buildAST(char *str,int len) {
	for (int i=0;i<len;++i) {
		Expr *expr_top=NULL;
		switch (str[i]) {
			case '|': case '+': {
				Expr *right_expr=expr_stk[expr_stk_top--];
				Expr *left_expr=expr_stk[expr_stk_top--];
				if (str[i]=='+') expr_top=newBinaryExpr(EXPR,left_expr,right_expr);
				else expr_top=newBinaryExpr(OREXPR,left_expr,right_expr);
				break;
			}
			case '*':{
				Expr *child_expr=expr_stk[expr_stk_top--];
				expr_top=newUnaryExpr(STAREXPR,child_expr,false,0);
				break;
			}
			default: expr_top=newUnaryExpr(EXPR,NULL,true,str[i]);
		}
		expr_stk[++expr_stk_top]=expr_top;
	}
	return expr_stk[expr_stk_top];
	// printf("%d\n",expr_stk_top);
	// printExpr(expr_stk[expr_stk_top],0);
}

void buildNFARecursive(NFA *nfa,Expr *expr,int delta) {
	if (expr->is_leaf) {
		if (addNFATrans(nfa,delta,delta+1,expr->ch)==-1) {
			// Error Handler
		}
		nfa->start=delta;
		nfa->end=delta+1;
		return ;
	}
	int start1=0,end1=0;
	switch (expr->type) {
		case EXPR: {
			buildNFARecursive(nfa,expr->left_expr,delta);
			start1=nfa->start,end1=nfa->end;

			nfa->start=nfa->end=0;
			buildNFARecursive(nfa,expr->right_expr,delta+expr->left_expr->size);
			addNFATrans(nfa,end1,nfa->start,0);
			nfa->start=start1;
			break;
		}
		case OREXPR: {
			buildNFARecursive(nfa,expr->left_expr,delta+2);
			start1=nfa->start,end1=nfa->end;

			nfa->start=nfa->end=0;
			buildNFARecursive(nfa,expr->right_expr,delta+2+expr->left_expr->size);
			int s2=nfa->start,e2=nfa->end;
			nfa->start=delta; nfa->end=delta+1;

			addNFATrans(nfa,delta,start1,0);
			addNFATrans(nfa,delta,s2,0);
			addNFATrans(nfa,end1,delta+1,0);
			addNFATrans(nfa,e2,delta+1,0);
			break;
		}
		case STAREXPR: {
			buildNFARecursive(nfa,expr->expr,delta+2);
			start1=nfa->start,end1=nfa->end;

			nfa->start=delta; nfa->end=delta+1;

			addNFATrans(nfa,delta,delta+1,0);
			addNFATrans(nfa,delta,start1,0);
			addNFATrans(nfa,end1,delta+1,0);
			addNFATrans(nfa,end1,start1,0);
		}
	}
}

NFA *buildNFA(Expr *root) {
	int size=calcNFASize(root);
	printf("size=%d\n",size);
	NFA *nfa=newNFA(size);
	buildNFARecursive(nfa,root,0);
	printNFA(nfa);
	return nfa;
}

int main(void) {
	char *str=(char*)calloc(sizeof(char),STACK_SIZE);
	char *ptr=(char*)calloc(sizeof(char),STACK_SIZE);
	scanf("%s",str);
	int len=strlen(str);
	int plen=proc(str,ptr,len);
	Expr *rt=buildAST(ptr,plen);
	NFA *nfa=buildNFA(rt);
	puts("-----");
	DFA *dfa=NFAtoDFA(nfa);
	printDFA(dfa,"origin_DFA");
	DFA *min_dfa=minDFA(dfa);
	printDFA(min_dfa,"minimized_DFA");
	while (true) {
		printf("RE:>");
		scanf("%s",str);
		if (str[0]=='@') {
			puts("Quit!");
			free(str);
			free(ptr);
			return 0;
		}
		dfa->state=1;
		bool flag=false;
		for (char *it=str;*it!='\0';it++) {
			if (moveDFA(dfa,(int)*it)==-1) {
				flag=true;
				break;
			}
		}
		if (!dfa->is_recv[dfa->state]) flag=false;
		puts(flag?"failed!":"matched!");
	}
	free(str);
	free(ptr);
	return 0;
}