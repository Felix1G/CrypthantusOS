#include "tss.h"
#include <string.h>

static TSS_ENTRY g_tss __attribute__((aligned(16)));

void tss_init(uint32_t kernel_stack_segment, uint32_t kernel_stack_pointer)
{
    memset(&g_tss, 0, sizeof(TSS_ENTRY));

    g_tss.ss0 = kernel_stack_segment;
    g_tss.esp0 = kernel_stack_pointer;
}

TSS_ENTRY* tss_address()
{
    return &g_tss;
}