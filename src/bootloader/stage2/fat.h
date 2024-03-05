#pragma once

#include <stdint.h>
#include "disk.h"

typedef struct
{
    uint8_t name[11];
    uint8_t attribute;
    uint8_t _reserved;
    uint8_t created_time_tenths;
    uint16_t created_time;
    uint16_t created_date;
    uint16_t accessed_date;
    uint16_t first_cluster_high;
    uint16_t modified_time;
    uint16_t modified_date;
    uint16_t first_cluster_low;
    uint32_t size;
} __attribute__((packed)) FAT_DIR_ENTRY;

typedef struct 
{
    int handle;
    bool is_dir;
    uint32_t pos;
    uint32_t size;
} FAT_FILE;

enum FILE_ATTRIBUTES
{
    FILE_ATTR_READ_ONLY = 0x1,
    FILE_ATTR_HIDDEN = 0x02,
    FILE_ATTR_SYSTEM = 0x04,
    FILE_ATTR_VOLUME_ID = 0x08,
    FILE_ATTR_DIRECTORY = 0x10,
    FILE_ATTR_ARCHIVE = 0x20,
    FILE_ATTR_LFN = FILE_ATTR_READ_ONLY | FILE_ATTR_HIDDEN | FILE_ATTR_SYSTEM | FILE_ATTR_VOLUME_ID
};

int fat_init(DISK *disk);
FAT_FILE *fat_open(DISK *disk, const char *path);
unsigned fat_read(DISK *disk, FAT_FILE *file, unsigned n, void *data);
bool fat_entry(DISK *disk, FAT_FILE *file, FAT_DIR_ENTRY *entry);
void fat_close(FAT_FILE *file);