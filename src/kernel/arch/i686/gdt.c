#include "io.h"
#include "gdt.h"
#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint16_t    limit_low;
    uint16_t    base_low;
    uint8_t     base_mid;
    uint8_t     access;
    uint8_t     limit_hi_flags;
    uint8_t     base_hi;
} __attribute__((packed)) GDT_ENTRY;

typedef struct
{
    uint16_t size;
    GDT_ENTRY* addr;
} __attribute((packed)) GDT_DESC;

typedef enum
{  
    GDT_ACCESS_ACCESSED_BIT = 0x01,

    GDT_ACCESS_CODE_READ = 0x02,
    GDT_ACCESS_DATA_WRITE = 0x02,

    GDT_ACCESS_CODE_CONFORMING = 0x04,
    GDT_ACCESS_DATA_DIR_NORM = 0x00,
    GDT_ACCESS_DATA_DIR_DOWN = 0x04,

    GDT_ACCESS_EXECUTABLE = 0x08,

    GDT_ACCESS_DESC_TSS = 0x00,
    GDT_ACCESS_DESC_NOT_TSS = 0x10,

    GDT_ACCESS_CODE_SEG = GDT_ACCESS_DESC_NOT_TSS | GDT_ACCESS_EXECUTABLE,
    GDT_ACCESS_DATA_SEG = GDT_ACCESS_DESC_NOT_TSS,

    GDT_ACCESS_RING0 = 0x00,
    GDT_ACCESS_RING1 = 0x20,
    GDT_ACCESS_RING2 = 0x40,
    GDT_ACCESS_RING3 = 0x60,

    GDT_ACCESS_PRESENT = 0x80,
} GDT_ACCESS;

typedef enum
{
    GDT_FLAG_64_BIT = 0x20,
    GDT_FLAG_32_BIT = 0x40,
    GDT_FLAG_16_BIT = 0x00,

    GDT_FLAG_GRANULARITY_1B = 0x00,
    GDT_FLAG_GRANULARITY_4K = 0x80
} GDT_FLAG;

#define GDT_LIMIT_LOW(limit) (limit & 0xFFFF)
#define GDT_BASE_LOW(base) (base & 0xFFFF)
#define GDT_BASE_MID(base) ((base >> 16) & 0xFF)
#define GDT_LIMIT_HI_FLAGS(limit, flags) (((limit >> 16) & 0xF) | (flags & 0xF0))
#define GDT_BASE_HIGH(base) ((base >> 24) & 0xFF)
#define GDT_ENTRY(base, limit, access, flags) {         \
    GDT_LIMIT_LOW(limit),                               \
    GDT_BASE_LOW(base),                                 \
    GDT_BASE_MID(base),                                 \
    access,                                             \
    GDT_LIMIT_HI_FLAGS(limit, flags),                   \
    GDT_BASE_HIGH(base),                                \
}

static GDT_ENTRY gdt_entry[6] = {
    GDT_ENTRY(0, 0, 0, 0), //null descriptor
    //ring 0 descriptor
    GDT_ENTRY(0, 0xFFFFF,
              GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | 
              GDT_ACCESS_CODE_SEG | GDT_ACCESS_CODE_READ | GDT_ACCESS_ACCESSED_BIT,
              GDT_FLAG_32_BIT | GDT_FLAG_GRANULARITY_4K),
    GDT_ENTRY(0, 0xFFFFF,
              GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | 
              GDT_ACCESS_DATA_SEG | GDT_ACCESS_DATA_WRITE | GDT_ACCESS_ACCESSED_BIT,
              GDT_FLAG_32_BIT | GDT_FLAG_GRANULARITY_4K),
};

GDT_DESC gdt_desc = { sizeof(gdt_entry) - 1, gdt_entry };

void __attribute__((cdecl)) i686_gdt_load(GDT_DESC* desc, uint16_t code_seg, uint16_t data_seg);

void i686_gdt_init()
{   
    i686_gdt_load(&gdt_desc, i686_GDT_CODE_SEG, i686_GDT_DATA_SEG);
}
