#include "buddy_alloc.h"
#include "hal/hal.h"
#include <stdint.h>
#include <stdio.h>

#define BUDDY_LAYERS 14
#define BUDDY_ARRAY_SIZE (1 << BUDDY_LAYERS) >> 3
#define BUDDY_PAGE_TABLES 256
#define TOTAL_PAGE_MAPPABLE_SIZE (4 * 1024 * 1024)

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
void* g_buddy_start; //NOTE: this is in physical address

static uint32_t g_buddy_page_tables[1024 * BUDDY_PAGE_TABLES] __attribute__((aligned(4096)));

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
        g_buddy_start = (void*)((size_t)(begin->begin + 
            begin->length + 1 - g_buddy_max_block) & 0xFFFFF000); //align at 4096 bytes

        int index = 0;
        for (int i = 0; i < g_buddy_max_block; i += TOTAL_PAGE_MAPPABLE_SIZE)
        {
            size_t block_size = 0;
            //starting at virtual address 0x80000000
            uint32_t table_phys = hal_physical_addr((uint32_t)(g_buddy_page_tables + (index * 1024)));
            hal_set_page_table((uint32_t*)table_phys, 512 + index, 0);
            //put the tables into an identity map
            hal_set_page_table_entry(hal_get_page_table(0), (table_phys >> 12) & 0x3FF, table_phys, 0, 0);
            index += 1;
        }
        
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
    while (min_block_size < size)
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
                
                int address_size = (g_buddy_min_block << (g_buddy_layers - min_layer - 1));
                size_t address = (address_size) * idx;
                for (int i = 0; i < address_size; i += 4096)
                { //g_buddy_page_tables itself is already aligned with the buddy allocator
                    hal_set_page_table_entry(g_buddy_page_tables + (address >> 22 << 10), ((address >> 12) & 0x3FF),
                        address + (size_t)g_buddy_start, 0, 0); //address uses physical address
                    address += 4096;
                }

                return (void*)((size_t)min_block_size * (size_t)idx + BUDDY_ALLOC_VIRADR); //return the virtual address
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
            return (void*)BUDDY_ALLOC_VIRADR;
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

    size_t min_block_idx = ((char*)ptr - (char*)BUDDY_ALLOC_VIRADR) / g_buddy_min_block;
    
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

    int address_size = (g_buddy_min_block << (g_buddy_layers - layer - 1));
    size_t address = (address_size) * min_block_idx;
    for (int i = 0; i < address_size; i += 4096)
    { //g_buddy_page_tables itself is already aligned with the buddy allocator
        hal_set_page_table_entry(g_buddy_page_tables + (address >> 22 << 10),
            ((address >> 12) & 0x3FF), 0x00, 0, 0); //address uses physical address
        address += 4096;
    }
}