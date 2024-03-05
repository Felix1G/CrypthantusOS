#include "idt.h"
//#include <util/binary.h>

typedef struct
{
    uint16_t base_low;
    uint16_t seg_selector;
    uint8_t _res;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed)) IDT_ENTRY;

typedef struct
{
    uint16_t size;
    IDT_ENTRY* addr;
} __attribute__((packed)) IDT_DESC;

IDT_ENTRY idt_entries[256];

IDT_DESC idt_desc = { sizeof(idt_entries) - 1, idt_entries };

void __attribute__((cdecl)) i686_idt_load(IDT_DESC* idt_desc);

void i686_idt_init()
{
    i686_idt_load(&idt_desc);
}

void i686_idt_set_present_flag(int intr)
{
    idt_entries[intr].flags |= IDT_FLAG_PRESENT;
}

void i686_idt_unset_present_flag(int intr)
{
    idt_entries[intr].flags &= ~IDT_FLAG_PRESENT;
}

void i686_idt_descriptor(int intr, void* base, uint16_t seg_desc, uint8_t flags)
{
    idt_entries[intr].base_low = ((uint32_t)base) & 0xFFFF;
    idt_entries[intr].seg_selector = seg_desc;
    idt_entries[intr]._res = 0;
    idt_entries[intr].flags = flags;
    idt_entries[intr].base_hi = ((uint32_t)base >> 16) & 0xFFFF;
}
