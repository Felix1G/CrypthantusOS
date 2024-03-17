#include "stdlib.h"
#include <string.h>
#include <util/buddy_alloc.h>

#define HEAP_FIRST_FIT_MAX_SIZE (8 * 1024)
#define HEAP_METADATA_SIZE (16 * 1024)
typedef size_t* szpt; //size pointer

typedef struct
{
    szpt alloc_root; //value points to the first heap object
    szpt free_root; //value points to the first empty space

    size_t avail_size; //cache variable declaration

    size_t best_avail_size;
    szpt best_alloc_ptr;
    szpt best_prev_free_ptr;
    szpt best_free_ptr;

    szpt alloc_ptr;
    szpt prev_free_ptr;
    szpt free_ptr;
    szpt temp_ptr;

    szpt block;
} HEAP_CACHE;

HEAP_CACHE* heapc;
szpt heap_blocks; //memory blocks initialised for the memory
size_t heap_blocks_size;

int heap_header_size; //heap header size
size_t block_size; //for buddy allocation

void heap_set_mem_block(szpt block, szpt adr, size_t size)
{
    block[0] = (size_t)adr;
    block[1] = (size_t)size;
}

szpt heap_mem_block(szpt ptr)
{
    for (int i = 0;i < heap_blocks_size;i += 2)
    {
        if (heap_blocks[i] == 0)
            continue;

        //if ptr is inside the heap block
        if ((size_t)ptr >= (size_t)heap_blocks[i] &&
            (size_t)ptr < (size_t)heap_blocks[i] + (size_t)heap_blocks[i + 1])
            return (heap_blocks + i);
    }

    return NULL;
}

szpt heap_find_empty_block_ptr()
{
    for (int i = 0;i < heap_blocks_size;i += 2)
    {
        if (heap_blocks[i] == 0)
            return (heap_blocks + i);
    }

    return NULL; //out of blocks
}

szpt heap_mem_block_end(szpt block)
{
    return (szpt)((void*)heapc->block[0] + (size_t)heapc->block[1]);
}

//memory block to check is heapc->block
int heap_not_in_mem_block(szpt ptr)
{
    return (size_t)ptr < (size_t)heapc->block[0] || (size_t)ptr >= (size_t)heap_mem_block_end(heapc->block);
}

//memory block to check is heapc->block
int heap_in_mem_block(szpt ptr) { return !heap_not_in_mem_block(ptr); }

//heap is always expected to be sorted in ascending order for individual memory blocks:
// [1 5 10] [2 7 8] [15 16 17] [18 19 24] [3 25]
int heap_init()
{
    heap_header_size = sizeof(size_t);
    heapc = buddy_alloc(HEAP_METADATA_SIZE, &block_size);

    if (heapc == NULL)
        return 0;
    else
    {
        heap_blocks_size = (block_size - sizeof(HEAP_CACHE)) / heap_header_size;
        size_t mem_blocks_size = heap_blocks_size * heap_header_size;

        heap_blocks = (szpt)((void*)heapc + (size_t)block_size - mem_blocks_size);

        return mem_blocks_size;
    }
}

void* malloc(size_t size)
{
    if (size == 0)
        return NULL;

    size += heap_header_size; //size of header
    
    if (heapc->alloc_root == 0)
    { //heap is empty
        szpt ptr = (szpt)buddy_alloc(size, &block_size); //allocate memory block for this heap obj
        if (ptr == 0) return 0; //no memory can be allocated

        heap_set_mem_block(heap_blocks, ptr, (size_t)block_size);

        heapc->alloc_root = ptr;

        //point to null
        *heapc->alloc_root = 0x00;
        if (block_size - size > heap_header_size)
        { //block is large enough for the free header
            heapc->free_root = ((void*)ptr + size);
            *heapc->free_root = 0x00;
        }
        
        return (void*)ptr + heap_header_size; //move after header
    }
    else
    {
        int first_fit = size < HEAP_FIRST_FIT_MAX_SIZE;
        heapc->best_avail_size = 2e9; //set variable regardless if it's first fit

        heapc->alloc_ptr = (szpt)&heapc->alloc_root;
        heapc->prev_free_ptr = (szpt)&heapc->free_root;
        heapc->free_ptr = heapc->free_root;

        heapc->block = NULL;
        
        while (heapc->free_ptr)
        { //so many blocks and available spaces
            //initialise the corresponding heap block
            if (heapc->block == NULL || heap_not_in_mem_block(heapc->free_ptr))
            {
                heapc->block = heap_mem_block(heapc->free_ptr);
                if (heapc->block == NULL)
                    break;
            }

            //find the alloc ptr closest to be linked with the new alloc object
            //note that this is only if the *alloc_ptr is in the same block
            //otherwise, the code handled for different block is at a while loop below
            if (heapc->alloc_ptr != 0)
                while (heap_in_mem_block((szpt)*heapc->alloc_ptr) &&
                        *heapc->alloc_ptr < (size_t)heapc->free_ptr)
                {
                    if (*heapc->alloc_ptr != 0)
                        heapc->alloc_ptr = (szpt)*heapc->alloc_ptr;
                    else
                        break;
                }

            //find available size in relation with the alloc ptr
            if (heap_in_mem_block((szpt)*heapc->alloc_ptr))
            { //alloc pointer is at another block, we get the current available size
                heapc->avail_size = (size_t)((void*)*heapc->alloc_ptr - (size_t)heapc->free_ptr);
            }
            else if (heap_mem_block(heapc->free_ptr) == heapc->block &&
                heap_mem_block((szpt)*heapc->alloc_ptr) != heapc->block) //block must be the same
            {
                heapc->avail_size = (size_t)(
                    (void*)heap_mem_block_end(heapc->block) -
                        (size_t)heapc->free_ptr);
                        
                if (size <= heapc->avail_size)
                { //look through alloc ptr until it reaches the block of the free pointer
                    register int not_in_block = 0;
                    while (heapc->alloc_ptr && (
                        (heap_not_in_mem_block((szpt)heapc->alloc_ptr) &&
                        (not_in_block = heap_not_in_mem_block((szpt)*heapc->alloc_ptr))) ||
                            (!not_in_block && *heapc->alloc_ptr < (size_t)heapc->free_ptr)))
                    {
                        if (heapc->alloc_ptr && *heapc->alloc_ptr != 0)
                            heapc->alloc_ptr = (szpt)*heapc->alloc_ptr;
                        else
                            break;
                    }
                }
            }
            else
                goto hmalloc_loop_continue;

            if (size <= heapc->avail_size)
            { //allocate into this memory block
                if (heapc->avail_size < heapc->best_avail_size)
                { //best fit
                    heapc->best_avail_size = heapc->avail_size;
                    heapc->best_alloc_ptr = heapc->alloc_ptr;
                    heapc->best_free_ptr = heapc->free_ptr;
                    heapc->best_prev_free_ptr = heapc->prev_free_ptr;

                    if (first_fit)
                        goto malloc_allocate_in_between;
                }
            }

            if (heapc->free_ptr != 0)
            {
                heapc->prev_free_ptr = (szpt)*heapc->prev_free_ptr;
                heapc->free_ptr = (szpt)*heapc->prev_free_ptr;
                continue;
            }

hmalloc_loop_continue:
            if (!heapc->free_ptr || *heapc->free_ptr == 0)
                break;
            heapc->free_ptr = (szpt)*heapc->free_ptr;
        }

        if (first_fit || heapc->best_avail_size == 2e9)
        {
            //create a new heap/memory block
            szpt empty_block_ptr = heap_find_empty_block_ptr();
            if (empty_block_ptr == NULL)
                return NULL;
            
            szpt ptr = (szpt)buddy_alloc(size, &block_size); //allocate memory block for this heap obj
            if (ptr == 0) return 0;

            heap_set_mem_block(empty_block_ptr, ptr, (size_t)block_size);

            //loop until the last allocation pointer (alloc_ptr is linked at the end root)
            heapc->alloc_ptr = heapc->alloc_root;
            if (heapc->alloc_ptr != 0)
                while (*heapc->alloc_ptr != 0)
                {
                    heapc->alloc_ptr = (szpt)*heapc->alloc_ptr;
                }
            *heapc->alloc_ptr = (size_t)ptr;
            
            //relink the free pointers
            if (block_size - size > heap_header_size) {
                if (heapc->free_ptr)
                {
                    *heapc->free_ptr = (size_t)((void*)ptr + size);
                    **(szpt*)heapc->free_ptr = 0x00; //free_ptr -> 0x00
                }
                else if (heapc->prev_free_ptr)
                {
                    *heapc->prev_free_ptr = (size_t)((void*)ptr + size);
                    **(szpt*)heapc->prev_free_ptr = 0x00; //free_ptr -> 0x00
                }
            }

            //point to null
            *ptr = 0x00;
            
            return (void*)ptr + heap_header_size; //move after header
        }
        else
        {
malloc_allocate_in_between:
            heapc->temp_ptr = (szpt)*heapc->best_free_ptr;
            *heapc->best_free_ptr = *heapc->best_alloc_ptr; //new heap object points to the next object
            *heapc->best_alloc_ptr = (size_t)heapc->best_free_ptr; //point to the new object

            if (heapc->best_avail_size - size > heap_header_size)
            {
                if ((void*)heapc->best_free_ptr + size + heap_header_size == (void*)*heapc->best_free_ptr)
                { //heap object has taken this empty space fully ( alloc | full | alloc )
                    *heapc->best_prev_free_ptr = (size_t)heapc->temp_ptr;
                }
                else
                { // ( alloc | alloc [some free space] | alloc )
                    *(szpt)((void*)*heapc->best_alloc_ptr + size) = (size_t)heapc->temp_ptr;
                    *heapc->best_prev_free_ptr = (size_t)((void*)*heapc->best_alloc_ptr + size);
                }
            }
            else if (heapc->best_prev_free_ptr)
            {
                *heapc->best_prev_free_ptr = (size_t)heapc->temp_ptr;
            }

            return ((void*)*(szpt)heapc->best_alloc_ptr) + heap_header_size; //move after header
        }
    }

    return NULL;
}

void* zmalloc(size_t size)
{
    uint8_t* data = (uint8_t*)malloc(size);
    while (size--) *data = 0x00;
    return (void*)data;
}

void* realloc(void* ptr, size_t size)
{
    if (ptr == NULL || heapc->alloc_root == 0)
        return NULL;

    void* real_ptr = ptr - heap_header_size; //header

    //get the allocation pointer
    heapc->alloc_ptr = heapc->alloc_root;
    while (heapc->alloc_ptr && heapc->alloc_ptr != real_ptr)
    {
        heapc->alloc_ptr = (szpt)*heapc->alloc_ptr;
    }
    if (heapc->alloc_ptr == 0x00)
        return NULL; //allocation header not found

    //get the next free allocation pointer to determine the size
    heapc->block = heap_mem_block((szpt)real_ptr);
    heapc->prev_free_ptr = (szpt)&heapc->free_root;
    heapc->free_ptr = (szpt)*heapc->prev_free_ptr;
    while (heap_in_mem_block(heapc->free_ptr) && (void*)heapc->free_ptr < real_ptr)
    {
        heapc->prev_free_ptr = (szpt)*heapc->prev_free_ptr;
        heapc->free_ptr = (szpt)*heapc->prev_free_ptr;
    }

    if (heap_in_mem_block((szpt)*heapc->alloc_ptr))
        heapc->avail_size = (void*)(szpt)*heapc->alloc_ptr - (void*)real_ptr;
    else
        heapc->avail_size = (void*)heap_mem_block_end(heapc->block) - (void*)real_ptr;
    if (heapc->avail_size > size + heap_header_size)
    { //extend the free header
        size += heap_header_size; //add by header size

        if (heapc->free_ptr != NULL)
        {
            if (heapc->avail_size <= size + heap_header_size)
            { //available size is not enough for a free header
                *heapc->prev_free_ptr = *heapc->free_ptr;
            }
            else
            {
                *(szpt)(real_ptr + size) = *heapc->free_ptr;
                *heapc->prev_free_ptr = (size_t)(real_ptr + size);
            }
        }

        return ptr;
    }
    else
    { //move the memory
        //add by header to validate pointer for the free function
        free(ptr);
        size_t ptr_size = heapc->avail_size;

        void* new_ptr = malloc(size);

        if (new_ptr != ptr)
            memcpy(new_ptr, ptr, ptr_size - heap_header_size);

        return new_ptr;
    }

    return NULL;
}

void free(void* ptr)
{
    if (ptr == NULL)
        return;

    ptr -= heap_header_size; //subtract by header size

    if ((heapc->block = heap_mem_block(ptr)) == NULL)
        return; //ptr is not in any of the memory blocks

    //find alloc ptr
    if (heapc->alloc_root == ptr)
    { //the first heap object by position is the ptr to free
        heapc->alloc_root = (szpt)*heapc->alloc_root;
        if (heapc->alloc_root == 0)
        {
            heapc->alloc_root = 0x00; //heap is now empty
            heapc->free_root = 0x00;
            goto free_clear_blocks;
        }
        heapc->alloc_ptr = (szpt)&heapc->alloc_root;
    }
    else
    {
        heapc->alloc_ptr = (szpt)&heapc->alloc_root; //reference can also handle 1 pointer in a block
        while (heapc->alloc_ptr && *heapc->alloc_ptr != 0 &&
            *heapc->alloc_ptr != (size_t)ptr)
            heapc->alloc_ptr = (szpt)*heapc->alloc_ptr;

        if (heapc->alloc_ptr != 0)
        {
            //it is expected that alloc ptr is just before the ptr to free
            if (*heapc->alloc_ptr != 0)
                *heapc->alloc_ptr = **(szpt*)heapc->alloc_ptr;
        }
        else
            return; //invalid alloc ptr
    }

    //reconnect the free pointers
    if (heap_in_mem_block(heapc->free_root) && ptr < (void*)heapc->free_root)
    {
        *(szpt)ptr = (size_t)heapc->free_root;
        heapc->free_root = (szpt)ptr;

        //there is a free pointer in front of this ptr, hence requires merge
        if (!( heapc->alloc_ptr && heapc->free_root && //right sep conditions
                (heap_not_in_mem_block((szpt)*heapc->free_root) ||
                (heap_in_mem_block((szpt)*heapc->alloc_ptr) &&
                *heapc->alloc_ptr < *heapc->free_root)) ) && *heapc->free_root != 0)
        {
            *heapc->free_root = **(size_t**)heapc->free_root;
        }
    }
    else if (heapc->free_root == 0x00)
    { //no free space
        heapc->free_root = ptr;
        *heapc->free_root = 0x00;
    }
    else
    { //ptr is somewhere far, far away
        heapc->free_ptr = heapc->free_root;
        while (heap_not_in_mem_block((szpt)*heapc->free_ptr) || *heapc->free_ptr < (size_t)ptr)
        { //make free_ptr lie before ptr
            if (*heapc->free_ptr != 0)
                heapc->free_ptr = (szpt)*heapc->free_ptr;
            else
                break;
        }
        
        int left_sep = //there is an alloc ptr at the left
            heap_not_in_mem_block(heapc->free_ptr) ||
            (heap_in_mem_block(heapc->alloc_ptr) &&
                heapc->alloc_ptr >= heapc->free_ptr);
        int right_sep = //there is an alloc ptr at the right
            heapc->alloc_ptr && heapc->free_ptr &&
                (heap_not_in_mem_block((szpt)*heapc->free_ptr) ||
                (heap_in_mem_block((szpt)*heapc->alloc_ptr) &&
                *heapc->alloc_ptr < *heapc->free_ptr));
        
        //f - free, a - alloc, n - freed alloc ptr
        if (left_sep)
        {
            if (right_sep)
            { //fanaf
                *(szpt)ptr = *heapc->free_ptr;
                *heapc->free_ptr = (size_t)ptr;
            }
            else
            { //fanf
                if (*heapc->free_ptr != 0)
                    *(szpt)ptr = **(szpt*)heapc->free_ptr;
                *heapc->free_ptr = (size_t)ptr;
            }
        }
        else
        { //NOTE: fnaf is just the same thing as faf
            //fnf
            if (!right_sep && *heapc->free_ptr != 0)
                *heapc->free_ptr = **(szpt*)heapc->free_ptr;
        }
    }

free_clear_blocks:
    //clear unused memory blocks
    heapc->block = heap_mem_block(ptr);

    heapc->alloc_ptr = heapc->alloc_root;
    do
    {
        //an alloc ptr is in the block. Do not clear this block.
        if (heap_in_mem_block(heapc->alloc_ptr))
            return;

        if (heapc->alloc_ptr)
            heapc->alloc_ptr = (szpt)*heapc->alloc_ptr;
        else
            break;
    } while (heapc->alloc_ptr != heapc->alloc_root); //prevent possible loop backs

    //clear this block
    buddy_free((void*)*heapc->block);

    heapc->prev_free_ptr = (szpt)&heapc->free_root;
    heapc->free_ptr = heapc->free_root;

    int found_ptr = 0;
    do
    {
        if (heap_in_mem_block(heapc->free_ptr))
        { //an alloc ptr is in the block. Do not clear this block.
            *heapc->prev_free_ptr = *heapc->free_ptr;
            heapc->free_ptr = (szpt)*heapc->free_ptr;
            found_ptr = 1;
        }
        else if (found_ptr)
            break;

        if (heapc->free_ptr) {
            heapc->prev_free_ptr = (szpt)*heapc->prev_free_ptr;
            heapc->free_ptr = (szpt)*heapc->prev_free_ptr;
        } else
            break;
    } while (heapc->free_ptr != heapc->free_root);

    //cleaning
    heap_set_mem_block(heapc->block, 0, 0);
}

void _heap_debug()
{
    printf("| Debug: -------------------\n| ");
    heapc->alloc_ptr = heapc->alloc_root;
    int i = 1024;
    do
    {
        printf("%X ", heapc->alloc_ptr);
        if (heapc->alloc_ptr)
            heapc->alloc_ptr = (szpt)*heapc->alloc_ptr;
    } while (heapc->alloc_ptr && i--);
    printf("%s\n| ", i == -1 ? "(Too many addresses)" : "");

    heapc->free_ptr = heapc->free_root;
    i = 1024;
    do
    {
        printf("%X ", heapc->free_ptr);
        if (heapc->free_ptr)
            heapc->free_ptr = (szpt)*heapc->free_ptr;
    } while (heapc->free_ptr && i--);
    printf("%s\n| ", i == -1 ? "(Too many addresses)" : "");

    for (int i = 0;i < heap_blocks_size;i += 2)
    {
        if (heap_blocks[i] != 0)
            printf("[%X %i] ", heap_blocks[i], heap_blocks[i + 1]);
    }
    printf("%s\n", i == -1 ? "(Too many addresses)" : "");
}