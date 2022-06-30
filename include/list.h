#ifndef __LIST_H__

#define __LIST_H__

#include "utils.h"
#include "debug.h"

#define member(type, node, field) \
  (((type *)node)->field)

#define next(type, node) \
  ((type *)(((node_t *)node)->next))

#define foreach(type, list, it) \
  for (type *it = ((type *)((list).head)); it != NULL; it = next(type, it))

struct node {
  struct node *next, *prev;
} ;

struct list {
  struct node *head, *tail;
  const char *name;
  int size;
} ;

typedef struct node node_t;
typedef struct list list_t;
// circ lists
typedef struct list clist_t;

// initialize list
void list_init(list_t *list, const char *name);

// list->lock should be held
void list_insert_after(list_t *list, node_t *node, node_t *prev);

// list->lock should be held
void list_insert_before(list_t *list, node_t *node, node_t *next);

// list->lock should be held
void list_insert_front(list_t *list, node_t *node);

// list->lock should be held
void list_insert_back(list_t *list, node_t *node);

// should hold list->lock, !(list->empty)
node_t *list_pop_front(list_t *list);

// should hold list->lock, !(list->empty)
node_t *list_pop_back(list_t *list);

// should hold list->lock, 
void list_delete(list_t *list, node_t *node);

// sort list, cmp(x, y) = [x <= y]
void list_sort(list_t *list, int (*cmp)(node_t *, node_t *));

bool empty(list_t *list);

void clist_insert(clist_t *clist, node_t *node);

bool clist_delete(clist_t *clist, node_t *node);

node_t *clist_pop(clist_t *clist);

void list_move(list_t *from, list_t *to);

// duplicate list from `from` to `to`
void list_copy(list_t *from, list_t *to, int size);

void list_check(list_t *list);

#endif