/**
 * Stack memory is bounded by the Buddy Allocator's memory
*/
#pragma once

typedef struct
{
    unsigned elem_size;
    unsigned alloc_size;
    void* sp;
    void* min;
} STACK;

STACK* stack_init(unsigned size, unsigned elem_size);
void stack_destroy(STACK* stack);
int stack_push(STACK* stack, ...); //0 success, 1 fail
void* stack_pop(STACK* stack);