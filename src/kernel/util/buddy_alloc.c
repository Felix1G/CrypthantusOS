#include "buddy_alloc.h"
#include <stdint.h>
#include <stdio.h>

#define BUDDY_LAYERS 14
#define BUDDY_ARRAY_SIZE (1 << BUDDY_LAYERS) >> 3

int buddy_layers[16] = { //extra one for the loop in alloc
    0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767
};

uint8_t g_buddy_block[BUDDY_ARRAY_SIZE];

int buddy_block(int layer, int idx)
{
    int index = buddy_layers[layer] + idx;
    int arr_idx = index >> 3;
    index &= 0b111;
    
    return !!(g_buddy_block[arr_idx] & (1 << index));
}

void buddy_block_set(int layer, int idx, int val)
{
    int index = buddy_layers[layer] + idx;
    int arr_idx = index >> 3;
    index &= 0b111;

    g_buddy_block[arr_idx] = (g_buddy_block[arr_idx] & ~(1 << index)) | (val << index);
}

int g_buddy_max_block;
int g_buddy_min_block;
int g_buddy_layers;
void* g_buddy_start;

int buddy_init(BOOT_DATA* boot_data)
{
    int max_size = 0;
    MEMORY_BLOCK* begin = 0;

    for (int i = 0; i < boot_data->memory.blocks_count; i++)
    {
        register MEMORY_BLOCK block = boot_data->memory.blocks[i];
        if (block.type != MEMORY_BLOCK_TYPE_USABLE)
            continue;
            
        register int size = block.length;
        if (size > max_size)
        {
            max_size = size;
            begin = boot_data->memory.blocks + i;
        }
    }

    if (begin)
    {
        max_size >>= 1; //take the second half
        
        int layers = 0;
        while (max_size)
        {
            max_size >>= 1;
            layers++;
        }

        g_buddy_max_block = 1 << layers >> 1; //>> 1 is intentional
        g_buddy_min_block = g_buddy_max_block >> max(0, min(layers - 2, BUDDY_LAYERS - 1));
        g_buddy_layers = min(layers, BUDDY_LAYERS);

        //safely allocate the memory at the end of the memory block
        //to avoid collision with the kernel's opcodes
        g_buddy_start = (void*)(size_t)(begin->begin + begin->length + 1 - g_buddy_max_block); 
        
        return g_buddy_max_block;
    }

    return 0;
}

void *buddy_alloc(size_t size, size_t* block_size)
{
    if (size > g_buddy_max_block)
        goto buddy_alloc_ret_0;

    if (buddy_block(0, 0) && !(buddy_block(1, 0) || buddy_block(1, 1)))
    {
        goto buddy_alloc_ret_0; //out of memory
    }
    
    int min_layer = g_buddy_layers - 1;
    int min_block_size = g_buddy_min_block;
    while (size > min_block_size)
    {
        min_block_size <<= 1;
        min_layer--;
    }

    int idx = 0;
    int search_up = 1;
    for (int layer = min_layer;layer >= 0;layer--)
    {
buddy_alloc_top_loop:
        if (layer <= 1 && buddy_block(1, 0) && buddy_block(1, 1))
            goto buddy_alloc_ret_0;

        register int idx_max = 1 << layer;
        for (idx;idx < idx_max;idx += 2)
        {
            if (layer != 0)
            {
                if (buddy_block(layer - 1, idx >> 1))
                { //top is occupied
                    if (search_up && !(buddy_block(layer, idx) || buddy_block(layer, idx + 1)))
                    { //only the parent block is allocated
                        continue;
                    }
                    else if (!buddy_block(layer, idx))
                    {
                        goto buddy_alloc_loop;
                    }
                    else if (!buddy_block(layer, idx + 1))
                    {
                        idx++;
                        goto buddy_alloc_loop;
                    }
                    else
                    { //both buddies are occupied
                        continue;
                    }
                }
                else
                { //top is empty
                    continue; //try another buddy
                }
            }

buddy_alloc_loop:
            if (buddy_block(layer, idx))
                continue; //this block is occupied

            if (layer == min_layer)
            { //block to allocate found
                buddy_block_set(layer, idx, 1);
                *block_size = g_buddy_min_block << (g_buddy_layers - layer - 1);
                return (void*)(min_block_size * idx + g_buddy_start);
            }
            else
            { //move deeper
                buddy_block_set(layer, idx, 1);
                layer += 1;
                idx <<= 1;

                search_up = 0;

                goto buddy_alloc_top_loop;
            }
        }

        idx = 0;
    }

    if (min_layer <= 0)
    { //size is the max block
        if (buddy_block(0, 0))
            goto buddy_alloc_ret_0; //block occupied
        else {
            buddy_block_set(0, 0, 1);
            *block_size = g_buddy_max_block;
            return g_buddy_start;
        }
    }
    else
    { //out of memory
buddy_alloc_ret_0:
        *block_size = 0;
        return 0;
    }
}

void buddy_free(void *ptr)
{
    if (!ptr)
        return; //NULL case

    size_t min_block_idx = ((char*)ptr - (char*)g_buddy_start) / g_buddy_min_block;
    
    int layer = g_buddy_layers - 1;
    while (!buddy_block(layer, min_block_idx))
    { //move up to find the occupied block
        layer--;
        min_block_idx >>= 1;
    }
    
    //set occupied block to unoccupied
    buddy_block_set(layer, min_block_idx, 0);

    //if its buddy is also unoccupied, set the parent block to be unoccupied
    while (layer >= 0 && !( buddy_block(layer, min_block_idx & ~1) || buddy_block(layer, (min_block_idx & ~1) + 1) ))
    {
        layer--;
        min_block_idx >>= 1;
        buddy_block_set(layer, min_block_idx, 0);
    }
}