#include <stdlib.h>
#include <assert.h>

#include "stack.h"

bool __stack_empty(stack_t *stack) {
    return (stack->size == 0);
}

void *__stack_pop(stack_t *stack) {
    assert(stack->size > 0);
    assert(stack->top != NULL);
    void *top = stack->top;
    stack->top = stack->top->pre;
    stack->size --;
    assert(stack->size == 0 || stack->top != NULL);
    return top;
}

void *__stack_top(stack_t *stack) {
    return stack->top;
}

void __stack_push(stack_t *stack, stack_node_t *node) {
    node->pre = stack->top;
    stack->top = node;
    stack->size ++;
}
