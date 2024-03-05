#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint8_t id;
    uint16_t heads;
    uint16_t cylinders;
    uint16_t sectors;
} DISK;

int disk_init(DISK *disk, uint8_t drive_number);
int disk_read_sectors(DISK *disk, uint32_t lba, uint8_t sectors, void *data);