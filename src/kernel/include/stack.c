#include "stack.h"
#include "string.h"
#include <stdint.h>
#include <util/buddy_alloc.h>

STACK* stack_init(unsigned size, unsigned elem_size)
{
    int block_size;
    void* alloc_ptr = buddy_alloc(size, block_size);
    size = block_size;
    if (!alloc_ptr)
        return 0;

    STACK* stack = (STACK*)((uint8_t*)alloc_ptr + size - sizeof(STACK)); //allocate stack here
    stack->elem_size = elem_size;
    stack->alloc_size = size;
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
