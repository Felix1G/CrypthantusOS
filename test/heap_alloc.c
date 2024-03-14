#include <stdio.h>
#include <string.h>
#include "buddy_allocator.h"

//test case 1: basic allocation and free
//test case 2: small allocation sizes
//test case 3: not enough space for free space headers
//test case 4: big allocation sizes
//test case 5: 'real world' case scenario
//test case 6: REALLOC
//test case 7: ultimate realloc testing
//test case 8: read files and print its reversed content
//test case 9: spamming 1 byte data

void heap_init();
void* heap_malloc(size_t size);
void* heap_calloc(size_t nitems, size_t size);
void* heap_realloc(void* ptr, size_t size);
void heap_free(void* ptr);
void heap_debug();

#define DEBUGON

void test_case_1()
{
    //a .
    int* a = heap_malloc(5 * sizeof(int)); //empty case
    heap_debug();
    *a = 60;
    //a b .
    int* b = heap_calloc(50, sizeof(int)); //second object case
    heap_debug();
    *b = 360;
    //a b c .
    int* c = heap_malloc(5 * sizeof(int)); //third object case
    heap_debug();
    *c = 2147483647;
    //a b c . | d .
    int* d = heap_calloc(15000, sizeof(int)); //new block case
    heap_debug();
    *d = 1234567;

    printf("%X, %i\n", a, *a);
    printf("%X, %i\n", b, a[7]);
    printf("%X, %i\n", c, a[59]);
    printf("%X, %i [%i]\n", d, d[0], (void*)d - (void*)c);

    //a . c . | d .
    heap_free(b);
    heap_debug();

    //a e . c . | d .
    int* e = heap_malloc(5 * sizeof(int)); //fit block case
    heap_debug();
    *e = 429;
    printf("%X, %i\n", e, *b);

    //a e f . c . | d .
    int* f = heap_malloc(5 * sizeof(int)); //fit block case
    heap_debug();
    *f = 123123;
    printf("%X, %i\n", f, b[7]);

    //. e f . c . | d .
    heap_free(a);
    heap_debug();
    //. e f . | d .
    heap_free(c);
    heap_debug();

    //. e f g . | d .
    int* g = heap_malloc(50 * sizeof(int)); //fit block case
    heap_debug();
    *g = 238329;
    printf("%X, %i\n", g, *g);

    //. e f g .
    heap_free(d); //buddy free case
    heap_debug();

    //. e f g . | h .
    int* h = heap_malloc(5000 * sizeof(int)); //fit block case
    heap_debug();
    printf("%X, %i\n", h, *d);

    //. e f g .
    heap_free(h);
    heap_debug();

    //. f g .
    heap_free(e);
    heap_debug();
    //. g .
    heap_free(f);
    heap_debug();
    //[empty]
    heap_free(g);
    heap_debug();
    //TODO test first fit and best fit scenarios
}

void test_case_2()
{
    //a .
    int* a = heap_malloc(2 * sizeof(int));
    heap_debug();
    *a = 170;
    //a b .
    int* b = heap_calloc(50, sizeof(int));
    heap_debug();
    *b = 180;
    //a b c .
    int* c = heap_malloc(3 * sizeof(int));
    heap_debug();
    *c = 190;
    //a b c d .
    int* d = heap_calloc(4, sizeof(int));
    heap_debug();
    *d = 200;
    //a b c d e .
    int* e = heap_calloc(4, sizeof(int));
    heap_debug();
    *e = 210;

    printf("%i %i %i %i %i ", *a, *b, *c, *d, *e);
    heap_free(b); //a . c d e .
    heap_debug();
    heap_free(d); //a . c . e .
    heap_debug();

    //a g . c f e .
    int* f = heap_calloc(3, sizeof(int));
    heap_debug();
    *f = 220;

    int* g = heap_calloc(3, sizeof(int));
    heap_debug();
    *g = 230;

    heap_free(a);
    heap_free(c);
    heap_free(e);
    heap_free(f);
    heap_free(g);
    heap_debug();

    printf("%i %i\n", *f, *g);
}

void test_case_3()
{
    int* a = heap_calloc(4094, sizeof(int)); //16376 bytes, free header cannot fit
    heap_debug(); *a=123;
    printf("%i\n", *a);

    int* b = heap_calloc(4094, sizeof(int)); //16376 bytes, free header cannot fit
    heap_debug(); *b=124;
    printf("%i\n", *b);

    int* c = heap_calloc(4094, sizeof(int)); //16376 bytes, free header cannot fit
    heap_debug(); *c = 232;
    printf("%i\n", *c);

    heap_free(b);
    heap_debug();

    int* d = heap_calloc(4094, sizeof(int)); //16376 bytes, free header cannot fit
    heap_debug(); *d= 129302;
    printf("%i\n", *d);

    char* e = heap_calloc(1, sizeof(char)); //1 byte
    heap_debug(); *e = 'a';
    printf("%i\n", *e);

    int* f = heap_calloc(100, sizeof(char)); //100 byte
    heap_debug(); *f = 20;
    printf("%i\n", *f);
    
    heap_free(a);
    heap_free(c);
    heap_free(d);
    heap_free(e);
    heap_free(f);
    heap_debug();
}

void test_case_4()
{
    int* a = heap_malloc(8e6);
    heap_debug();
    printf("%X\n", a);

    int* b = heap_malloc(5);
    heap_debug();
    printf("%X\n", b);

    int* c = heap_malloc(512e6);
    heap_debug();
    printf("%X\n", c);

    heap_free(c);
    heap_free(a);
    heap_free(b);
    heap_debug();
}

void test_case_5()
{
    int* mat1 = (int*)heap_calloc(10 * 10, sizeof(int));
    int* mat2 = (int*)heap_calloc(10 * 10, sizeof(int));
    int* matRes = (int*)heap_calloc(10 * 10, sizeof(int));

    for (int x = 0;x < 10;x++)
    {
        for (int y = 0;y < 10;y++)
        {
            mat1[10 * x + y] = rand() % 5;
            mat2[10 * x + y] = rand() % 5;
        }
    }

    for (int y = 0;y < 10;y++)
    {
        for (int x = 0;x < 10;x++)
        {
            printf("%i\t", mat1[x + 10 * y]);
        }
        printf("\n");
    }
    printf("\n");

    for (int y = 0;y < 10;y++)
    {
        for (int x = 0;x < 10;x++)
        {
            printf("%i\t", mat2[x + 10 * y]);
        }
        printf("\n");
    }
    printf("\n");

    for (int y = 0;y < 10;y++)
    {
        for (int x = 0;x < 10;x++)
        {
            int c = 0;
            for (int i = 0; i < 10;i++)
            {
                c += mat1[y * 10 + i] * mat2[i * 10 + x];
            }
            printf("%i\t", c);
        }
        printf("\n");
    }
    printf("\n");

    heap_debug();
    heap_free(mat1);
    heap_free(mat2);
    heap_free(matRes);
}

void test_case_6()
{
    int* a = (int*)heap_malloc(50 * sizeof(int));
    heap_debug();
    int* b = (int*)heap_malloc(50 * sizeof(int));
    heap_debug();
    a = (int*)heap_realloc(a, 500 * sizeof(int));
    heap_debug();
    b = (int*)heap_realloc(b, 60 * sizeof(int));
    heap_debug();
    int* c = (int*)heap_malloc(50 * sizeof(int));
    heap_debug();
    int* d = (int*)heap_malloc(50 * sizeof(int));
    heap_debug();
    int* e = (int*)heap_malloc(50 * sizeof(int));
    heap_debug();

    heap_free(d);
    heap_debug();

    c = (int*)heap_realloc(c, 80 * sizeof(int));
    heap_debug();

    printf("%X %X %X %X %X\n", a, b, c, d, e);

    heap_free(a);
    heap_debug();
    heap_free(b);
    heap_debug();
    heap_free(c);
    heap_debug();
    heap_free(e);
    heap_debug();
}

void test_case_7()
{
    char* str = NULL;
char buf[1000000];
    int debt = 0;

    while (1)
    {
        if (debt == 0)
            scanf("%s", &buf);
        
        int len = strlen(buf);
        int slen = 0;

        if (!memcmp(buf, "stop", 4)) {
            heap_free(str);
            heap_debug();
            return;
        }

        if (buf[0] == 's')
        {
            for (int i = 0;i < 99999;i++)
                buf[i] = rand() % 20 + 'a';
            buf[99999] = '\0';
            buf[0] = 'a';
            len = 100000;
        } 

        if (buf[0] == 'r' || debt)
        {
            if (debt == 0)
                scanf("%i", &debt);
            int pay = 1000000 < debt ? 1000000 : debt;
            for (int i = 0;i < pay;i++)
                buf[i] = rand() % 20 + 'a';
            buf[pay] = '\0';
            buf[0] = 'a';
            len = pay;
            debt -= pay;
        } 
        
        if (str == NULL)
        {
            str = heap_malloc(len * sizeof(char));
        }
        else
        {
            slen = strlen(str);
            str = heap_realloc(str, slen + len * sizeof(char));
        }

        memcpy(str + slen, buf, len);

        if (buf[0] == 'a')
            printf("%i %X\n", strlen(str), str);
        else
            printf(str);

        heap_debug();
    }
}

void test_case_8()
{
    FILE* fa = fopen("/home/thefelixks/a.txt", "r");
    FILE* fb = fopen("/home/thefelixks/b.txt", "r");
    FILE* fc = fopen("/home/thefelixks/c.txt", "r");
    if (fa == NULL || fb == NULL || fc == NULL)
    {
        printf("ERROR: NO FILES?");
        return;
    }

    long pos = ftell(fa);
    fseek(fa, 0, SEEK_END);
    long sa = ftell(fa);
    fseek(fa, pos, SEEK_SET);

    pos = ftell(fb);
    fseek(fb, 0, SEEK_END);
    long sb = ftell(fb);
    fseek(fb, pos, SEEK_SET);

    pos = ftell(fc);
    fseek(fc, 0, SEEK_END);
    long sc = ftell(fc);
    fseek(fc, pos, SEEK_SET);

    char* stra = (char*)heap_malloc(sa);
    char* strb = (char*)heap_malloc(sb);
    char* strc = (char*)heap_malloc(sc);
    char* stra1 = (char*)heap_malloc(sa);
    char* strb1 = (char*)heap_malloc(sb);
    char* strc1 = (char*)heap_malloc(sc);

    fread(stra, sizeof(char), sa, fa);
    fread(strb, sizeof(char), sb, fb);
    fread(strc, sizeof(char), sc, fc);

    for (int i = 0;i < sa-1;i++)
        stra1[sa - 2 - i] = stra[i];
    for (int i = 0;i < sb-1;i++)
        strb1[sb - 2 - i] = strb[i];
    for (int i = 0;i < sc-1;i++)
        strc1[sc - 2 - i] = strc[i];

    stra1[sa-1] = '\0';
    strb1[sb-1] = '\0';
    strc1[sc-1] = '\0';

    printf("FILE A:\n%s\n\nFILE B:\n%s\n\nFILE C:\n%s\n\n", stra1, strb1, strc1);
    heap_debug();

    heap_free(stra);
    heap_free(strb);
    heap_free(strc);
    heap_free(stra1);
    heap_free(strb1);
    heap_free(strc1);
    heap_debug();
}

void test_case_9()
{
    for (int i = 0;i < 1000;i++)
        heap_malloc(1);
    heap_debug();
}

int main()
{
    buddy_init();
    heap_init();

    test_case_1();
    printf("\n\n\n\n\n\n\n-----------------TESTCASE2----------------\n");
    test_case_2();
    printf("\n\n\n\n\n\n\n-----------------TESTCASE3----------------\n");
    test_case_3();
    printf("\n\n\n\n\n\n\n-----------------TESTCASE4----------------\n");
    test_case_4();
    printf("\n\n\n\n\n\n\n-----------------TESTCASE5----------------\n");
    test_case_5();
    printf("\n\n\n\n\n\n\n-----------------TESTCASE6----------------\n");
    test_case_6();
    printf("\n\n\n\n\n\n\n-----------------TESTCASE7----------------\n");
    test_case_7();
    printf("\n\n\n\n\n\n\n-----------------TESTCASE8----------------\n");
    test_case_8();
    printf("\n\n\n\n\n\n\n-----------------TESTCASE9----------------\n");
    test_case_9();
}

#define min(a, b) (a < b ? a : b)
#define HEAP_FIRST_FIT_MAX_SIZE (8 * 1024)
#define HEAP_BLOCKS_SIZE 256 * 2 //256 blocks, [address size] (2)

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

    szpt* block; //intentional double pointer
} HEAP_CACHE;

HEAP_CACHE* heapc;
szpt heap_blocks[HEAP_BLOCKS_SIZE]; //memory blocks initialised for the heap (256 blocks, 512 / 2)
int heap_header_size; //heap header size
int block_size; //for buddy allocation

void heap_set_mem_block(szpt* block, szpt adr, size_t size)
{
    block[0] = adr;
    block[1] = (szpt)size;
}

szpt* heap_mem_block(szpt ptr)
{
    for (int i = 0;i < HEAP_BLOCKS_SIZE;i += 2)
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

szpt* heap_find_empty_block_ptr()
{
    for (int i = 0;i < HEAP_BLOCKS_SIZE;i += 2)
    {
        if (heap_blocks[i] == NULL)
            return (heap_blocks + i);
    }

    return NULL; //out of blocks
}

szpt heap_mem_block_end(szpt* block)
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
void heap_init()
{
    heap_header_size = sizeof(size_t);
    heapc = buddy_alloc(sizeof(HEAP_CACHE), &block_size);
}

void heap_debug()
{
#ifdef DEBUGON
    printf("| Debug: -------------------\n| ");
    heapc->alloc_ptr = heapc->alloc_root;
    int i = 1024;
    do
    {
        printf("%X ", heapc->alloc_ptr);
        if (heapc->alloc_ptr)
            heapc->alloc_ptr = *heapc->alloc_ptr;
    } while (heapc->alloc_ptr && i--);
    printf("%s\n| ", i == -1 ? "(Too many addresses)" : "");

    heapc->free_ptr = heapc->free_root;
    i = 1024;
    do
    {
        printf("%X ", heapc->free_ptr);
        if (heapc->free_ptr)
            heapc->free_ptr = *heapc->free_ptr;
    } while (heapc->free_ptr && i--);
    printf("%s\n| ", i == -1 ? "(Too many addresses)" : "");

    for (int i = 0;i < HEAP_BLOCKS_SIZE;i += 2)
    {
        if (heap_blocks[i] != NULL)
            printf("[%X %i] ", heap_blocks[i], heap_blocks[i + 1]);
    }
    printf("%s\n", i == -1 ? "(Too many addresses)" : "");
#endif
}

void* heap_malloc(size_t size)
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
                        goto heap_malloc_allocate_in_between;
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
            szpt* empty_block_ptr = heap_find_empty_block_ptr();
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
heap_malloc_allocate_in_between:
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

void* heap_calloc(size_t nitems, size_t size)
{
    size_t total_size = nitems * size;
    uint8_t* data = (uint8_t*)heap_malloc(total_size);
    while (total_size--) *data = 0x00;
    return (void*)data;
}

void* heap_realloc(void* ptr, size_t size)
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
                *(szpt)(ptr + size) = *heapc->free_ptr;
                *heapc->prev_free_ptr = (size_t)(ptr + size);
            }
        }

        return ptr;
    }
    else
    { //move the memory
        //add by header to validate pointer for the free function
        heap_free(ptr);
        size_t ptr_size = heapc->avail_size;

        void* new_ptr = heap_malloc(size);

        if (new_ptr != ptr)
            memcpy(new_ptr, ptr, ptr_size - heap_header_size);

        return new_ptr;
    }

    return NULL;
}

void heap_free(void* ptr)
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
            goto heap_free_clear_blocks;
        }
        heapc->alloc_ptr = &heapc->alloc_root;
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
                (heap_not_in_mem_block(*heapc->free_root) ||
                (heap_in_mem_block(*heapc->alloc_ptr) &&
                *heapc->alloc_ptr < *heapc->free_root)) ) && *heapc->free_root != 0)
        {
            *heapc->free_root = (szpt)**(szpt*)heapc->free_root;
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
                (heap_not_in_mem_block(*heapc->free_ptr) ||
                (heap_in_mem_block(*heapc->alloc_ptr) &&
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

heap_free_clear_blocks:
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