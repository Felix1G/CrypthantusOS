#include "stack.h"
#include "string.h"
#include <stdint.h>
#include <util/buddy_alloc.h>

#define STACK_ALLOC_SIZE 8192

STACK* stack_init(unsigned size)
{
    void* alloc_ptr = buddy_alloc(STACK_ALLOC_SIZE);
    STACK* stack = (STACK*)((uint8_t*)alloc_ptr + STACK_ALLOC_SIZE - sizeof(STACK)); //allocate stack here
    stack->elem_size = size;
    stack->sp = stack;
    stack->min = alloc_ptr;
    return stack;
}

void stack_destroy(STACK* stack)
{
    buddy_free(stack->min);
}

int stack_push(STACK* stack, ...)
{
    void* new_ptr = (uint8_t*) stack->sp - stack->elem_size;
    if ((uint8_t*)new_ptr >= (uint8_t*)stack->min)
    {
        memcpy(new_ptr, (void*)(&stack + 1), stack->elem_size);
        stack->sp = new_ptr;
        return 0;
    }
    return 1;
}

void* stack_pop(STACK* stack)
{
    void* new_ptr = ((uint8_t*) stack->sp) + stack->elem_size;
    if ((uint8_t*)new_ptr > (uint8_t*)stack)
        return 0;
    
    void* ret = stack->sp;
    stack->sp = new_ptr;
    return ret;
}
