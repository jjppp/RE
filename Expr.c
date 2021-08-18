#include "Expr.h"

Expr* newBinaryExpr(expr_t type,Expr *left_expr,Expr *right_expr) {
	Expr *expr=(Expr*)malloc(sizeof(Expr));
	expr->type=type;
	expr->is_leaf=false;

	expr->left_expr=left_expr;
	expr->right_expr=right_expr;
	return expr;
}

Expr* newUnaryExpr(expr_t type,Expr *child_expr,bool is_leaf,int ch) {
	Expr *expr=(Expr*)malloc(sizeof(Expr));
	expr->type=type;
	if (expr->is_leaf=is_leaf) {
		expr->ch=ch;
	}

	expr->expr=child_expr;
	return expr;
}

int calcNFASize(Expr *expr) {
	if (expr->is_leaf) {
		return expr->size=2;
	}
	int res=0;
	switch (expr->type) {
		case OREXPR:case EXPR: {
			int ls=calcNFASize(expr->left_expr);
			int rs=calcNFASize(expr->right_expr);
			expr->size=ls+rs+((expr->type==OREXPR)?2:0);
			return expr->size;
		}
		case STAREXPR:
			return expr->size=calcNFASize(expr->expr)+2;
	}
}

void printExpr(Expr *expr,int dep) {
	for (int i=0;i<dep;++i) putchar(' ');
	expr_t type=expr->type;
	printf("%d\n",type);
	if (expr->is_leaf) return ;
	switch (type) {
		case OREXPR:case EXPR: {
			printExpr(expr->left_expr,dep+1);
			printExpr(expr->right_expr,dep+1);
			break;
		}
		case STAREXPR:
			printExpr(expr->expr,dep+1);
	}
}