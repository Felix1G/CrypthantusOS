#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <boot/bootlib.h>

int disk_init(DISK *disk, uint8_t drive_number);
int disk_read_sectors(DISK *disk, uint32_t lba, uint8_t sectors, void *data);