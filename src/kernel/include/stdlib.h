#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void _heap_debug();

int heap_init(); //1 success, 0 failed

//Allocate memory as a storage for the data associated.
void* malloc(size_t size);

//Zeroes the allocated memory.
void* calloc(size_t nitems, size_t size);

//Change the size of the storage associated with the data.
void* realloc(void* ptr, size_t size);

//Free the storage associated with the data.
void free(void* ptr);