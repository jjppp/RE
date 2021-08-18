#include "Set.h"

// implement Set by Ascending Ordered LinkedList

Set* newSet() {
	Set *set=(Set*)malloc(sizeof(Set));
	set->head=set->tail=NULL;
	set->size=0;
	return set;
}

struct SetElem* newSetElem(elem_t elem) {
	struct SetElem* node=(struct SetElem*)malloc(sizeof(struct SetElem));
	node->next=node->prev=NULL;
	node->elem=elem;
	return node;
}

void insElem(struct SetElem *prev,struct SetElem *next, struct SetElem *node) {
	node->prev=prev; node->next=next;
	if (prev!=NULL) prev->next=node;
	if (next!=NULL) next->prev=node;
}

void insert(Set **set,elem_t elem) {
	struct SetElem *node=newSetElem(elem);
	(*set)->size++;

	if ((*set)->size==1) {
		(*set)->head=(*set)->tail=node;
		return ;
	}

	struct SetElem *it=(*set)->head;
	if (it->elem>elem) {
		// insElem head
		insElem(NULL,(*set)->head,node);
		(*set)->head=node;
		return ;
	} else if (it->elem==elem) return ;

	struct SetElem *prev=it;
	for (it=it->next;it!=NULL;it=it->next,prev=prev->next) {
		if (it->elem>elem) {
			insElem(prev,it,node);
			return ;
		} else if (it->elem==elem) return ;
	}

	// insElem tail
	insElem((*set)->tail,NULL,node);
	(*set)->tail=node;
}

bool isEuqal(Set *a,Set *b) {
	if (a->size!=b->size) return false;
	struct SetElem *ia=a->head,*ib=b->head;
	for (;ia!=NULL&&ib!=NULL;ia=ia->next,ib=ib->next) {
		if (ia->elem!=ib->elem) return false;
	}
	return true;
}

void printSet(Set *set) {
	for (struct SetElem *it=set->head;it!=NULL;it=it->next) {
		printf("%d ",it->elem);
	} puts("");
}
