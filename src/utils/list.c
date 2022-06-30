#include "list.h"
#include "string.h"

void list_insert_after(list_t *list, node_t *node, node_t *prev) {
  assert(node);
  assert(prev);

  *node = (node_t) {
    .next = prev->next,
    .prev = prev
  } ;

  prev->next = node;

  list->size ++;

  assert(prev->next == node);
  assert(node->prev == prev);
}

void list_insert_before(list_t *list, node_t *node, node_t *next) {
  assert(node);
  assert(next);

  *node = (node_t) {
    .next = next,
    .prev = next->prev
  } ;

  next->prev = node;
  list->size ++;

  assert(next->prev == node);
  assert(node->next == next);
}

void list_insert_front(list_t *list, node_t *node) {
  assert(list);
  assert(node);
  node->prev = node->next = NULL;

  if (!list->size) {
    list->head = list->tail = node;
  } else {
    assert(list->head);

    node->next = list->head;
    list->head->prev = node;
    list->head = node;
  }
  list->size ++;
}

void list_insert_back(list_t *list, node_t *node) {
  assert(list);
  if (!node) {
    assert(node);
  }
  assert(node);
  node->prev = node->next = NULL;

  if (!list->size) {
    list->tail = list->head = node;
  } else {
    assert(list->tail);

    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
  }
  list->size ++;
}

/** delete `node`
 * prev<->node<->next
 */
void list_delete(list_t *list, node_t *node) {
  assert(list);
  assert(node);
  assert(list->size > 0);

  node_t *prev = node->prev;
  node_t *next = node->next;
  if (prev) {
    prev->next = next;
  }
  if (next) {
    next->prev = prev;
  }
  node->prev = node->next = NULL;
}

node_t *list_pop_front(list_t *list) {
  assert(list->size > 0);

  node_t *front = list->head;
  if (list->size == 1) {
    list->head = list->tail = NULL;
  } else {
    list->head = front->next;
    list->head->prev = NULL;
  }
  front->next = front->prev = NULL;
  list->size --;
  return front;
}

node_t *list_pop_back(list_t *list) {
  assert(list->size > 0);

  node_t *back = list->tail;
  if (list->size == 1) {
    list->head = list->tail = NULL;
  } else {
    list->tail = back->prev;
    list->tail->next = NULL;
  }
  back->next = back->prev = NULL;
  list->size --;
  return back;
}

/** swap x, y
 *  prev<->x<->y<->next
 */
static void swap(node_t *x, node_t *y) {
  x->next = y->next;
  y->prev = x->prev;
  if (x->prev) {
    x->prev->next = x->next;
  }
  if (y->next) {
    y->next->prev = x;
  }
  x->prev = y;
  y->next = x;
}

void list_sort(list_t *list, int (*cmp)(node_t *, node_t *)) {
  if (list->size <= 1) {
    return ;
  }

  for (int i = 0; i < list->size; ++ i) {
    for (node_t *it = list->head; it->next; ) {
      node_t *next = it->next;
      if (!cmp(it, next)) {
        swap(it, next);
      } else {
        it = it->next;
      }
    }
  }
}

void clist_insert(clist_t *clist, node_t *node) {
  assert(clist);
  assert(clist->tail == NULL);
  assert(node);

  node_t *head = clist->head;
  if (!clist->size) {
    clist->head = node;
    node->next = node->prev = node;
  } else if (clist->size == 1) {
    node->next = node->prev = clist->head;
    head->next = node;
    head->prev = node;
  } else {
    /**  ______________
     *  |             |
     * head<->node<->next
     */
    node_t *next = head->next;
    assert(next != head);
    head->next = node;
    node->next = next;
    node->prev = head;
    next->prev = node;
  }

  clist->size ++;
}

node_t *clist_pop(clist_t *clist) {
  assert(clist);
  assert(clist->size > 1);

  clist->size --;

  node_t *front = clist->head;
  clist->head = clist->head->next;
  clist->head->prev = front->prev;
  front->prev->next = clist->head;

  front->prev = front->next = NULL;
  return front;
}

bool clist_delete(clist_t *clist, node_t *node) {
  assert(clist);
  assert(clist->size > 0);
  assert(node);

  node_t *head = clist->head;
  int cnt = clist->size;

  foreach(node_t, (*clist), it) {
    if (it == node) {
      if (clist->size == 1) {
        clist->head = NULL;
      } else {
        if (node == head) {
          clist->head = head->next;
        }

        node_t *prev = node->prev;
        node_t *next = node->next;

        prev->next = node->next;
        next->prev = node->prev;
      }
      node->next = node->prev = NULL;
      clist->size --;
    }
    cnt --; if (!cnt) return false;
  }
  return true;
}

void list_init(list_t *list, const char *name) {
  *list = (list_t) {
    .name = name, .size = 0,
    .head = NULL, .tail = NULL,
  } ;
}

void list_move(list_t *from, list_t *to) {
  assert(from);
  assert(to);

  to->head = from->head;
  to->tail = from->tail;
  to->size = from->size;

  from->head = NULL;
  from->tail = NULL;
  from->size = 0;
}

void list_check(list_t *list) {
  int size = 0;
  foreach(node_t, *list, it) {
    assert(!it->next || it->next->prev == it);
    assert(!it->prev || it->prev->next == it);
    size ++;
  }
  if (size != list->size) {
    assert(size == list->size);
  }
}

void list_copy(list_t *from, list_t *to, int size) {
  assert(from);
  assert(to);

  assert(to->size == 0);
  assert(to->head == NULL);
  assert(to->tail == NULL);

  if (!from->size) {
    return ;
  }
  assert(from->head);
  assert(from->tail);

  foreach(node_t, *from, it) {
    node_t *node = Malloc(size);
    memcpy(node, it, size);
    *node = (node_t) {
      .next = NULL, .prev = NULL
    } ;
    list_insert_back(to, node);
  }
}

bool empty(list_t *list) {
  return list->size == 0;
}