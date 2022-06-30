/* A Regular Expression Engine
* Valid Characters:
* (,),*,|
* @ stands for \epsilon
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "State.h"
#include "Expr.h"
#include "DFA.h"
#include "NFA.h"
#include "stack.h"
#include "debug.h"

#define DFA_SIZE 200005
#define STACK_SIZE 200005

typedef Expr *pExpr;

STACK_NODE_DEF(char);
STACK_NODE_DEF(pExpr);

int preProc(char *str,int len) {
    char *buf = strdup(str);
	int slen = 1;
    str[0] = buf[0];
    for (int i = 1; i < len; ++ i) {
		if (buf[i - 1] != '|'
         && buf[i - 1] != '('
         && buf[i] != '|'
         && buf[i] != '*'
         && buf[i] != ')') {
			str[slen ++] = '+';
		}
		str[slen ++] = buf[i];
	}
	str[slen] = 0;
    debug("%s\n", str);
    free(buf);
	return slen;
}

int proc(char *str,char *ptr,int len) {
    static const int prior[CHAR_SIZE] = {
        ['|'] = 1, ['+'] = 2, ['*'] = 3,
    };
    static stack_t char_stack;
    len = preProc(str, len);
    int plen = 0;
    for (int i = 0; i < len; ++ i) {
		switch (str[i]) {
			case '(':
                stack_push(char_stack, char, str[i]);
				break;
			case ')':
                while (stack_top(char_stack, char) != '(') {
                    ptr[plen ++] = stack_pop(char_stack, char);
                }
                stack_pop(char_stack, char);
				break;
			case '|':case'*':case '+':
                while (!stack_empty(char_stack)
                    &&  prior[(int) stack_top(char_stack, char)] >= prior[(int) stack_top(char_stack, char)] ) {
                    ptr[plen ++] = stack_pop(char_stack, char);
                }
                stack_push(char_stack, char, str[i]);
				break;
			default: ptr[plen++]=str[i];
		}
	}
    while (!stack_empty(char_stack)) {
        ptr[plen ++] = stack_pop(char_stack, char);
    }
	ptr[plen] = 0;
	printf("%s\n",ptr);
	return plen;
}

Expr* buildAST(char *str,int len) {
    static stack_t expr_stack;
	for (int i=0;i<len;++i) {
		Expr *expr_top=NULL;
		switch (str[i]) {
			case '|': case '+': {
                pExpr right_expr = stack_pop(expr_stack, pExpr);
                pExpr  left_expr = stack_pop(expr_stack, pExpr);

                if (str[i] == '+') {
                    expr_top = newBinaryExpr(EXPR, left_expr, right_expr);
                } else {
                    expr_top = newBinaryExpr(OREXPR, left_expr, right_expr);
                }
				break;
			}
			case '*':{
                pExpr child_expr = stack_pop(expr_stack, pExpr);
                expr_top = newUnaryExpr(STAREXPR, child_expr, false, 0);
				break;
			}
			default: expr_top=newUnaryExpr(EXPR,NULL,true,str[i]);
		}
        stack_push(expr_stack, pExpr, expr_top);
	}
    return stack_top(expr_stack, pExpr);
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
            break;
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: RE pattern");
        exit(1);
    }
    char *str = strdup(argv[1]);
	char *ptr=(char*)calloc(sizeof(char),STACK_SIZE);
	int len=strlen(str);
	int plen=proc(str,ptr,len);
	Expr *rt=buildAST(ptr,plen);
	NFA *nfa=buildNFA(rt);
	puts("-----");
	DFA *dfa=NFAtoDFA(nfa);
	printDFA(dfa,"origin_DFA");
	DFA *min_dfa=minDFA(dfa);
	printDFA(min_dfa,"minimized_DFA");
    for (printf("RE:>"); scanf("%s", str) != EOF; printf("RE:>")) {
		dfa->state=1;
		bool flag=false;
		for (char *it=str;*it != '\0';it++) {
			if (moveDFA(dfa,(int)*it)==-1) {
				flag=true;
				return 0;
			}
		}
		if (!dfa->is_recv[dfa->state]) flag=false;
		puts(flag?"failed!":"matched!");
		if (flag) return 0;
	}
	free(str);
	free(ptr);
	return 0;
}