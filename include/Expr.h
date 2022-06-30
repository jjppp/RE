#ifndef EXPR_H
#define EXPR_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {EXPR,OREXPR,STAREXPR} expr_t;

typedef struct Expr {
	expr_t type;
	union {
		struct Expr *left_expr;
		struct Expr *expr;
	} ;
	union {
		struct Expr *right_expr;
		int ch;
	} ;
	int size;
	bool is_leaf;
} Expr;

Expr* newBinaryExpr(expr_t type,Expr *left_expr,Expr *right_expr);

Expr* newUnaryExpr(expr_t type,Expr *child_expr,bool is_leaf,int ch);

void printExpr(Expr *expr,int dep);

int calcNFASize(Expr *expr);

#endif