#include <stdio.h>
#include "State.h"

int main(void) {
	int n; scanf("%d",&n);
	StateList *list1=newStateList();
	for (int i=1;i<=n;++i) {
		int x; scanf("%d",&x);
		insertState(list1,x);
	}
	int m; scanf("%d",&m);
	StateList *list2=newStateList();
	for (int i=1;i<=m;++i) {
		int x; scanf("%d",&x);
		insertState(list2,x);
	}

	mergeList(list1,list2);
	printList(list1);
	return 0;
}