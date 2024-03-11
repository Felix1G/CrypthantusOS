#include <stdio.h>
#include "buddy_allocator.h"

void heap_init();
void* heap_malloc(unsigned size);
void* heap_calloc(unsigned nitems, unsigned size);
void* heap_realloc(void* ptr, unsigned size);
void heap_free(void* ptr);

int main()
{
    buddy_init();
    heap_init();

    int* a = heap_malloc(5 * sizeof(int)); //empty case
    int* b = heap_malloc(50 * sizeof(int)); //second object case
    int* c = heap_malloc(5 * sizeof(int)); //third object case
    int* d = heap_malloc(5000 * sizeof(int)); //new block case
    *a = 60;
    printf("%X, %i\n", a, *a);
    *b = 360;
    printf("%X, %i\n", b, *b);

    heap_free(b);
    int* e = heap_malloc(5 * sizeof(int)); //fit block case
    heap_free(a);
    heap_free(c);
    heap_free(d);
    heap_free(e);
    //TODO test first fit and best fit scenarios
}

unsigned* heap_blocks;
int heap_block_size = 0;
uint8_t* heap_alloc_root = 0;
uint8_t* heap_free_root = 0;
uint8_t* heap_start = 0;

void heap_init()
{
    int block_size;
    heap_blocks = buddy_alloc(512 * sizeof(unsigned), &block_size);
}

void* heap_malloc(unsigned size)
{
    if (size == 0)
        return NULL;

    size += 4; //size of header
    
    if (heap_alloc_root == 0)
    { //heap is empty
        int block_size;
        uint8_t* ptr = (uint8_t*)buddy_alloc(size, &block_size);
        heap_blocks[heap_block_size++] = (unsigned)ptr;
        heap_blocks[heap_block_size++] = block_size;

        heap_alloc_root = ptr;
        heap_free_root = ptr + size;
        heap_start = ptr;

        //point to null
        *(unsigned*)ptr = 0x00;
        *(unsigned*)heap_free_root = 0x00;
        
        return (void*)ptr + 4; //move after header
    }
    else
    {
        if (heap_alloc_root == heap_start)
        { //there is a heap object the start of the heap
            uint8_t* alloc_ptr = heap_alloc_root;
            uint8_t* free_ptr = heap_free_root;
            
            if (*alloc_ptr == 0x00 && free_ptr != 0x00)
            { //there is only 1 heap object
                unsigned block_size = heap_blocks[1];
                unsigned available = heap_blocks[0] + block_size - (unsigned)free_ptr;
                
                if (size <= available)
                { //put the available here
                    *(unsigned**)alloc_ptr = free_ptr;
                    *free_ptr = 0x00; //point to null

                    if (available == size)
                        heap_free_root = 0x00;
                    else
                        heap_free_root = (uint8_t*)free_ptr + size;
                    
                    return (void*)*(unsigned**)alloc_ptr + 4; //move after header
                }
                else
                { //TODO create a new block

                }
            }
            else
            { //search until alloc or free ends
                do {
                    break;
                } while (*alloc_ptr != 0x00);
            }
        }
    }

    return NULL;
}

void* heap_calloc(unsigned nitems, unsigned size)
{
    return NULL;
}

void* heap_realloc(void* ptr, unsigned size)
{
    return NULL;
}

void heap_free(void* ptr)
{

}