#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void debug_pr();
void hinit();
void *halloc(size_t size);
void hfree(void* ptr);

int main() {
    hinit();
    
    int* obj = (int*)halloc(2*sizeof(int));
    *obj = 999;
    obj[1] = 6;
    
    int* x = (int*)halloc(5*sizeof(int));
    x[0] = 100;
    x[1] = 100;
    x[2] = 100;
    x[3] = 100;
    x[4] = 100;
    
    hfree(x);
    
    x = (int*)halloc(2*sizeof(int));
    if (x == NULL)
        printf("isisis");
    else {
        x[0] = 123;
        x[1] = 125;
    }
    
    int* f = (int*)halloc(50*sizeof(int));
    f[0] = 699;
    f[12] = 700;
    f[24] = 701;
    f[25] = 700;
    f[26] = 699;
    
   // hfree(obj);
    //hfree(x);
    hfree(f);
    
    int* k = (int*)halloc(51*sizeof(int));
    k[0] = -4647478;
    for (int i = 1;i < 51;i++)
    {
        k[i] = 100*i;
    }
    
    debug_pr();

    return 0;
}

typedef struct HEAP_NODE
{
    void* prev;
    void* next;
    void* begin;
    void* end;
} HEAP_NODE;

typedef struct
{
    HEAP_NODE* root;
    size_t objects;
} HEAP;

HEAP g_heap;
char* heap_start;

#define HEAP_SIZE 1000

void debug_pr()
{    
    for (int i=0;i<HEAP_SIZE/4;i++)
        printf("%i ", ((int*)heap_start)[i]);
}

void hinit()
{
    g_heap.objects = 0;
    heap_start = (char*)malloc(HEAP_SIZE);
}

void *halloc(size_t size)
{
    if (g_heap.objects)
    {
        //there is already a first fit before the root
        if ((char*)g_heap.root->begin - (char*)heap_start >= size)
        {
            HEAP_NODE* new_node = (HEAP_NODE*)malloc(sizeof(HEAP_NODE));
            new_node->prev = g_heap.root->next;
            new_node->next = g_heap.root->next;
            new_node->begin = heap_start;
            new_node->end = heap_start + size;
           
            ((HEAP_NODE*)new_node->next)->prev = (void*)new_node;
            ((HEAP_NODE*)new_node->prev)->next = (void*)new_node;
            g_heap.root = new_node;
                
            g_heap.objects++;
            return new_node->begin; 
        }
        
        register HEAP_NODE* node = g_heap.root->next;
        
        for (int i = 1;i < g_heap.objects;i++)
        {
            register unsigned empty_size = (char*)node->begin - (char*)((HEAP_NODE*)node->prev)->end;
            
            if (empty_size >= size)
            {
                HEAP_NODE* new_node = (HEAP_NODE*)malloc(sizeof(HEAP_NODE));
                new_node->prev = node;
                new_node->next = node->next;
                new_node->begin = ((HEAP_NODE*)node->prev)->end;
                new_node->end = (char*)new_node->begin + size;
                
                ((HEAP_NODE*)new_node->next)->prev = new_node;
                ((HEAP_NODE*)new_node->prev)->next = new_node;
                
                g_heap.objects++;
                return new_node->begin;
            }
            
            node = node->next;
        }
    }
    else //heap is empty
    {
        HEAP_NODE* node = (HEAP_NODE*)malloc(sizeof(HEAP_NODE));
        node->prev = node;
        node->next = node;
        node->begin = heap_start;
        node->end = heap_start + size;
            
        g_heap.root = node;
        g_heap.objects++;
         
        return node->begin;
    }
     
    HEAP_NODE* end_node = g_heap.root->prev;
    if (heap_start + HEAP_SIZE - (char*)end_node->end >= size)
    {
        HEAP_NODE* new_node = (HEAP_NODE*)malloc(sizeof(HEAP_NODE));
        new_node->prev = end_node;
        new_node->next = end_node->next;
        new_node->begin = end_node->end;
        new_node->end = (char*)end_node->end + size;
        
        ((HEAP_NODE*)new_node->next)->prev = new_node;
        ((HEAP_NODE*)new_node->prev)->next = new_node;
        
        g_heap.objects++;
        return new_node->begin;
    }
    
    return NULL;
}

void hfree(void* ptr)
{
    if (!g_heap.objects)
        return; //heap is empty
        
    HEAP_NODE* begin = g_heap.root;
    HEAP_NODE* cur = begin;
    
    if (cur->begin == ptr) {
        goto hfree_success;
    }
        
    do
    {
        cur = cur->next;
        if (cur->begin == ptr)
            goto hfree_success;
    } while (cur != begin);
    
    goto hfree_fail;
    
hfree_success:
    if (g_heap.objects == 1)
    { //cur is the last object
        g_heap.root = NULL;
        g_heap.objects = 0;
    }
    else
    {
        ((HEAP_NODE*)cur->prev)->next = cur->next;
        ((HEAP_NODE*)cur->next)->prev = cur->prev;
        
        if (cur == g_heap.root)
            g_heap.root = cur->next;
        
        g_heap.objects--;
    }
    
    free(cur);
    
hfree_fail:
    ;
}
