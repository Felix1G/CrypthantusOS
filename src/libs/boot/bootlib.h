#pragma once

#include <stdint.h>

typedef struct {
    uint64_t begin;
    uint64_t length;
    uint32_t type;
    uint32_t acpi;
} MEMORY_BLOCK;

typedef struct {
    MEMORY_BLOCK* blocks;
    int blocks_count;
} MEMORY_INFO;

typedef struct
{
    uint8_t id;
    uint16_t heads;
    uint16_t cylinders;
    uint16_t sectors;
} DISK;

typedef struct {
    MEMORY_INFO memory;
    DISK disk;
    uint8_t boot_device;
} BOOT_DATA;

enum MEMORY_BLOCK_TYPE
{
    MEMORY_BLOCK_TYPE_USABLE = 1,
    MEMORY_BLOCK_TYPE_RESERVED = 2,
    MEMORY_BLOCK_TYPE_ACPI_RECLAIMABLE = 3,
    MEMORY_BLOCK_TYPE_ACPI_NVS = 4,
    MEMORY_BLOCK_TYPE_BAD_MEMORY = 5
};