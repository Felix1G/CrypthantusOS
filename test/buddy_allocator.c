#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void buddy_init();
void *buddy_alloc(size_t size);
void buddy_free(void *ptr);

int* test(size_t size)
{
    int* a = buddy_alloc(size);
    if (a == NULL)
        printf("ERROR\n");
    else {
        a[0] = 1;
        a[1] = 10;
        a[2] = 100;
    }

    printf("0x%X\n", a);

    return a;
}

int main()
{
    buddy_init();

    int* a = test(256 * 1024 * 1024);
    int* b = test(1024);
    int* c = test(1024);
    int* d = test(1024);
    int* e = test(1024);
    buddy_free(b);
    int* f = test(1024);
    int* g = test(1024);
    b = test(256 * 1024 * 1024);
    buddy_free(g);

    return 0;
}

#define BUDDY_LAYER_SIZE 16
#define BUDDY_ARRAY_SIZE (1 << BUDDY_LAYER_SIZE) >> 3

int buddy_layers[BUDDY_LAYER_SIZE] = { //extra one for the loop in alloc
    0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383
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

const int g_buddy_max_block = 1024 * 1024 * 512; //512MB
const int g_buddy_min_block = g_buddy_max_block >> (BUDDY_LAYER_SIZE - 1);
void* g_buddy_start;

void buddy_init()
{
    g_buddy_start = malloc(g_buddy_max_block); //512MB
}

void *buddy_alloc(size_t size)
{
    if (size > g_buddy_max_block)
        return NULL;
    
    int min_layer = BUDDY_LAYER_SIZE - 1;
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
            return 0;

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
            if (layer == min_layer)
            { //block to allocate found
                buddy_block_set(layer, idx, 1);
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
            return 0; //block occupied
        else {
            buddy_block_set(0, 0, 1);
            return g_buddy_start;
        }
    }
    else
    { //out of memory
        return 0;
    }
}

void buddy_free(void *ptr)
{
    if (!ptr)
        return; //NULL case

    size_t min_block_idx = ((char*)ptr - (char*)g_buddy_start) / g_buddy_min_block;
    
    int layer = BUDDY_LAYER_SIZE - 1;
    while (!buddy_block(layer, min_block_idx))
    { //move up to find the occupied block
        layer--;
        min_block_idx >>= 1;
    }
    
    buddy_block_set(layer, min_block_idx, 0);
    printf("%i, %i\n", layer, buddy_block(layer, min_block_idx));

    while (layer >= 0 && !( buddy_block(layer, min_block_idx & ~1) || buddy_block(layer, (min_block_idx & ~1) + 1) ))
    {
        layer--;
        min_block_idx >>= 1;
        buddy_block_set(layer, min_block_idx, 0);
    }

    int* start = (int*)g_buddy_start;
    for (int i = 0;i < 10;i++)
        printf("%i ", start[i]);
    printf("| ");
    for (int i = 2048;i < 2058;i++)
        printf("%i ", start[i]);
    printf("| ");
    for (int i = 32768;i < 32778;i++)
        printf("%i ", start[i]);
    printf("\n");
}