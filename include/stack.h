#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>

typedef struct __stack_node_t {
    struct __stack_node_t *pre;
} stack_node_t;

typedef struct __stack_t {
    stack_node_t *top;
    size_t size;
} stack_t;

#define STACK_NODE_NAME(TYPE) \
    TYPE##_stack_node_t

#define STACK_NODE_DEF(TYPE) \
    typedef struct TYPE##_stack_node_t { \
        stack_node_t node;   \
        TYPE elem;           \
    } TYPE##_stack_node_t

#define stack_push(STACK, TYPE, ELEM)   \
    do {                                \
        STACK_NODE_NAME(TYPE) *__tmp_ptr = malloc(sizeof(STACK_NODE_NAME(TYPE))); \
        __tmp_ptr->elem = ELEM;         \
        __stack_push(&STACK, (void *)__tmp_ptr); \
    } while (0)

#define stack_pop(STACK, TYPE)          \
    (((STACK_NODE_NAME(TYPE) *)(__stack_pop(&STACK)))->elem) \

#define stack_top(STACK, TYPE)          \
    (((STACK_NODE_NAME(TYPE) *)(__stack_top(&STACK)))->elem) \

#define stack_empty(STACK) \
    __stack_empty(&STACK)

void *__stack_top(stack_t *stack); 

void *__stack_pop(stack_t *stack); 

void __stack_push(stack_t *stack, stack_node_t *node); 

bool __stack_empty(stack_t *stack);

#endif