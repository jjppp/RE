#include <stdlib.h>

#include "stack.h"

void stack_pop(stack_t *stack) {
    assert(stack->size > 0);
    assert(stack->top != NULL);
    stack->top = stack->top->pre;
    assert(stack->top != NULL);
    stack->size --;
}

stack_node_t *stack_top(stack_t *stack) {
    return stack->top;
}

void stack_push(stack_t *stack, stack_node_t *node) {
    node->pre = stack->top;
    stack->top = node;
    stack->size ++;
}
