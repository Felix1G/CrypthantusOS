#include "hal.h"
#include <arch/i686/io.h>
#include <arch/i686/page.h>
#include <arch/i686/tss.h>
#include <stddef.h>
#include <stdlib.h>

void (*g_keyboard_onpress_listener)(int ch, int up, const KEY_STATE* state) = NULL;

int isnotchar(int ch) {
    return ch == KEY_LSHIFT || ch == KEY_RSHIFT || ch == KEY_CAPS || ch == KEY_CTRL || ch == KEY_ALT ||
            ch == KEY_ESC || ch == KEY_DELETE || ch == KEY_PAGE_UP || ch == KEY_PAGE_DOWN || ch == KEY_F1 ||
            ch == KEY_F2 || ch == KEY_F3 || ch == KEY_F4 || ch == KEY_F5 || ch == KEY_F6 ||
            ch == KEY_F7 || ch == KEY_F8 || ch == KEY_F9 || ch == KEY_F10 || ch == KEY_F11 ||
            ch == KEY_F12 || ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT;
}

int us_keyboard_down[256] =
{
    0x0, KEY_ESC, '1', '2', '3', '4', '5', '6',                         // 0 - 7
    '7', '8', '9', '0', '-', '=', KEY_BACKSPACE, '\t',                  // 8 - 15
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',                             // 16 - 23
    'o', 'p', '[', ']', '\n', KEY_CTRL, 'a', 's',                       // 24 - 31
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',                             // 32 - 39
    '\'', '`', KEY_LSHIFT, '\\', 'z', 'x', 'c', 'v',                    // 40 - 47
    'b', 'n', 'm', ',', '.', '/', KEY_RSHIFT, '*',                      // 48 - 55
    KEY_ALT, ' ', KEY_CAPS, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,     // 56 - 63
    KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0x0, 0x0, '7',             // 64 - 71
    KEY_UP, KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', '1',       // 72 - 79
    KEY_DOWN, KEY_PAGE_DOWN, '0', '.', KEY_DELETE, 0x0, 0x0, KEY_F11,   // 80 - 87
    KEY_F12                                                             // 88
};

int us_keyboard_up[256] =
{
    0x0, KEY_ESC, '!', '@', '#', '$', '%', '^',                         // 0 - 7
    '&', '*', '(', ')', '_', '+', KEY_BACKSPACE, '\t',                  // 8 - 15
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',                             // 16 - 23
    'O', 'P', '{', '}', '\n', KEY_CTRL, 'A', 'S',                       // 24 - 31
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',                             // 32 - 39
    '\"', '~', KEY_LSHIFT, '|', 'Z', 'X', 'C', 'V',                     // 40 - 47
    'B', 'N', 'M', '<', '>', '?', KEY_RSHIFT, '*',                      // 48 - 55
    KEY_ALT, ' ', KEY_CAPS, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,     // 56 - 63
    KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0x0, 0x0, '7',             // 64 - 71
    KEY_UP, KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', '1',       // 72 - 79
    KEY_DOWN, KEY_PAGE_DOWN, '0', '.', KEY_DELETE, 0x0, 0x0, KEY_F11,   // 80 - 87
    KEY_F12                                                             // 88
};

KEY_STATE g_key_state;
int g_last_keyboard_char;
int g_last_keyboard_up;

void _hal_irq_keyboard(REGISTERS* regs)
{
    int ch = i686_io_keyboard_read();
    
    int up = ch & 0x80;
    if (!ch)
        return;
    
    ch &= 0x7F;
    if ((g_key_state.lshift || g_key_state.rshift) ^ g_key_state.caps_lock)
        ch = us_keyboard_up[ch];
    else
        ch = us_keyboard_down[ch];

    switch (ch)
    {
    case KEY_CAPS:
        int caps_on = !(up || g_key_state.caps_lock);
        int caps_off = !up && g_key_state.caps_lock;
        
        if (caps_on)
            g_key_state.caps_lock = true;
        else if (caps_off)
            g_key_state.caps_lock = false;

        break;
    case KEY_LSHIFT:
        g_key_state.lshift = !up;
        break;
    case KEY_RSHIFT:
        g_key_state.rshift = !up;
        break;
    case KEY_CTRL:
        g_key_state.ctrl = !up;
        break;
    case KEY_ALT:
        g_key_state.alt = !up;
        break;
    }

    g_last_keyboard_char = ch;
    g_last_keyboard_up = up;

    if (g_keyboard_onpress_listener != NULL)
        g_keyboard_onpress_listener(ch, up, &g_key_state);
}

int hal_keyboard_char() { return g_last_keyboard_char; }
int hal_keyboard_up() { return g_last_keyboard_up; }

void hal_keyboard_onpress(void (*listener)(int ch, int up, const KEY_STATE* state))
{
    g_keyboard_onpress_listener = listener;
}

extern uint32_t __kernel_end;
void _hal_irq_page_fault(REGISTERS* regs)
{
    if ((regs->err & 0b111) == 0b000)
    { //supervisory process tried to read a non-present page entry, create an entry
        uint32_t* page_table __attribute__((aligned(4096)));
        size_t block_size;
        page_table = (uint32_t*)buddy_alloc(4096, &block_size);

        i686_page_table(page_table, 1, PAGE_READ_WRITE | PAGE_PRESENT);
    }
    else
    {
        printf("[ERROR]: #PF Unhandled Error Code. (0x%8X)", regs->err);
        i686_panic();
    }
}

void syscall0(REGISTERS* regs)
{ //end function
    int process_num = hal_current_process_num();
    printf("[LOG]%s %i ended with exit code %i.",
        (hal_process_attribute(process_num) & PROCESS_THREAD) ? "Thread" : "Process",
        process_num, regs->ebx);
    hal_remove_current_process();
}

void syscall1(REGISTERS* regs)
{ //read
    readf((const char*)regs->ebx, (int*)regs->ecx);
}

void syscall2(REGISTERS* regs)
{ //write
    writef((const char*)regs->ebx, (int*)regs->ecx);
}

void syscall3(REGISTERS* regs)
{ //sleep
    hal_set_sleep_tick(hal_current_process_num(), regs->ebx);
    //TODO
}
void syscall4(REGISTERS* regs)
{ //malloc
    regs->ecx = (uint32_t)malloc(regs->ebx);
}

void syscall5(REGISTERS* regs)
{ //zmalloc
    regs->ecx = (uint32_t)zmalloc(regs->ebx);
}

void syscall6(REGISTERS* regs)
{ //zmalloc
    regs->edx = (uint32_t)realloc(regs->ecx, regs->ebx);
}

void syscall7(REGISTERS* regs)
{ //free
    free(regs->ebx);
}

/**
 * TODO syscall 1 read
 * TODO syscall 3 sleep
 * TODO syscall 4-7 heap memory saving
*/

typedef void (*SYSCALL_HANDLER)(REGISTERS* regs);
SYSCALL_HANDLER syscall_handlers[7] = {
    syscall0, syscall1, syscall2, syscall3, syscall4, syscall5, syscall6, syscall7
};

volatile int i = 0;

void _hal_isr_syscall(REGISTERS* regs)
{
    if (regs->eax >= 0 && regs->eax <= 3)
        syscall_handlers[regs->eax](regs);
    else
        ; //run software exception signal
    
    i++;
    if (i % 140000== 0) {
        printf("[LOG]You have made a System Call. Congratz :D %i", regs->eax);
        regs->ecx = 12030;
    }
}
