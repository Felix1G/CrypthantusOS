#include "hal.h"
#include <stdlib.h>
#include <string.h>
#include <arch/i686/io.h>
#include <arch/i686/gdt.h>
#include <arch/i686/idt.h>
#include <arch/i686/irq.h>
#include <arch/i686/tss.h>
#include <arch/i686/page.h>
#include <arch/i686/floppy.h>

#define MAX_THREADS 10
#define USER_SPACE_SIZE 0xA00000 //12MB
#define USER_SPACE_4MBs 3        //12MB / 4

extern void __attribute__((cdecl)) disable_intr();
extern void __attribute__((cdecl)) enable_intr();
extern uint32_t __attribute__((cdecl)) getflags();
extern void __attribute__((cdecl)) save_to_pcb(size_t pcb_address, size_t eip_addr);
extern int __attribute__((cdecl)) load_pcb(size_t pcb_address, int usermode);

typedef struct
{
    REGISTERS regs;
    int valid;
} PCB;

// [xrtpaaaa]
// aaaa = 4b parent process (if thread)
// p = present
// t = thread
// r = prioritised
volatile uint8_t processes[MAX_THREADS];
volatile PCB pcbs[MAX_THREADS];
volatile PCB kernel_pcb;
volatile size_t sleep_ticks[MAX_THREADS];
volatile int cur_running_process = -1; //-1 = kernel

uint8_t hal_process_attribute(int index)
{
    return processes[index];
}

int hal_current_process_num()
{
    return cur_running_process;
}

void hal_remove_current_process()
{
    if (cur_running_process == -1)
        return; //cannot stop kernel

    processes[cur_running_process] = processes[cur_running_process] & (~PROCESS_PRESENT);
    memset((pcbs + cur_running_process), 0, sizeof(REGISTERS));
}

int hal_set_sleep_tick(int index, int ticks)
{
    sleep_ticks[index] = ticks;
}

int find_process(int process_idx, int check_present)
{
    for (process_idx;process_idx < MAX_THREADS;process_idx++)
    {
        if (!!(check_present) == !!(processes[process_idx] & PROCESS_PRESENT))
            return process_idx;
    }

    return -1;
}

int preempt(REGISTERS* regs, int ticks)
{
    if (ticks <= 5 && cur_running_process >= 0 &&
        (processes[cur_running_process] & PROCESS_THREAD))
    {
        return 0;    
    }

    if (cur_running_process == -1)
        memcpy(&kernel_pcb.regs, regs, sizeof(REGISTERS));
    else if (processes[cur_running_process] & PROCESS_PRESENT)
        memcpy(&((pcbs + cur_running_process)->regs), regs, sizeof(REGISTERS));

    int prev_process = cur_running_process;
    if (cur_running_process == -1)
        cur_running_process = find_process(0, 1);
    else
        cur_running_process = find_process(cur_running_process + 1, 1);

    if (prev_process == cur_running_process)
        return 0;
    
    if (cur_running_process == -1)
    { //run kernel
        memcpy(regs, &kernel_pcb.regs, sizeof(REGISTERS));
    }
    else
    { //run task
        if (!pcbs[cur_running_process].valid)
        { //invalid pcb, validate by setting zero values to regs
            REGISTERS* pcbreg = &(pcbs[cur_running_process].regs);
            pcbreg->cr0 = regs->cr0;
            pcbreg->cr2 = regs->cr2;
            pcbreg->cr3 = regs->cr3;
            pcbreg->cr4 = regs->cr4;
            pcbreg->ebp = regs->ebp;
            pcbreg->ebx = regs->ebx;
            pcbreg->ecx = regs->ecx;
            pcbreg->edx = regs->edx;
            pcbreg->edi = regs->edi;
            pcbreg->esi = regs->esi;
            pcbreg->intr = regs->intr;
            pcbreg->err = regs->err;
            pcbreg->oldesp = regs->oldesp;
            pcbs[cur_running_process].valid = 1;
        }

        memcpy(regs, &(pcbs[cur_running_process].regs), sizeof(REGISTERS));
    }

    return 1;
}

int hal_create_user_process(void (*user_func)(), int prio)
{
    hal_disable_interrupt();

    int block_size;

    //find free thread space
    int process_idx = find_process(0, 0);
    if (process_idx == -1)
        return 0;
    processes[process_idx] |= PROCESS_PRESENT | (prio ? PROCESS_PRIO : 0x00);

    //get physical address
    uint32_t phys = (uint32_t)hal_physical_addr((uint32_t)user_func);
    uint32_t user_entry_phys = (uint32_t)hal_physical_addr((uint32_t)i686_user_entry);
    uint32_t* table = (uint32_t*)buddy_alloc(4096, &block_size);

    //set table entry
    uint32_t table_idx = 1 + USER_SPACE_4MBs * process_idx;
    hal_set_page_table((uint32_t*)hal_physical_addr((uint32_t)table), table_idx, 1);
    for (int i = 0;i < 1024;i++)
        hal_set_page_table_entry(table, i, phys + (i * 0x1000), 1, 0);

    if (!i686_is_ring3_tss_gdt_init()) {
        printf("[LOG]Setting up userspace for the first time.");
        i686_ring3_tss_gdt_init(i686_kernel_stack_segment(), i686_kernel_stack_pointer());

        i686_preempt_handler(preempt);

        //also set the page for the user entry at 0x100000 - 0x104000
        for (int i = 256;i < 257;i++)
            hal_set_page_table_entry(hal_get_page_table(0), i, user_entry_phys + ((i - 256) * 0x1000), 1, 1);
    }

    printf("[LOG]Running Process %i.", process_idx);

    REGISTERS* pcb_regs = &(pcbs[process_idx].regs);
    pcb_regs->ds = (4 * 8) | 3;
    pcb_regs->es = (4 * 8) | 3;
    pcb_regs->fs = (4 * 8) | 3;
    pcb_regs->gs = (4 * 8) | 3;

    pcb_regs->ss = (4 * 8) | 3;
    pcb_regs->esp = ((table_idx + 1) << 22) - 1;
    pcb_regs->eflags = getflags() | 0x200; // | enable interrupt
    pcb_regs->cs = (3 * 8) | 3;
    pcb_regs->eip = 0x100000 | (user_entry_phys & 0xFFF);
    
    pcb_regs->eax = (table_idx << 22) | (phys & 0xFFF); //user function
    pcbs[process_idx].valid = 0;
    
    hal_enable_interrupt();

    return 1;
}