#ifndef __STACK_H__
#define __STACK_H__

typedef struct __stack_node_t {
    struct __stack_node_t *pre;
} stack_node_t;

typedef struct __stack_t {
    stack_node_t *top;
    size_t size;
} stack_t;

stack_node_t *stack_top(stack_t *stack); 

void stack_pop(stack_t *stack); 

void stack_push(stack_t *stack, stack_node_t *node); 

#endif