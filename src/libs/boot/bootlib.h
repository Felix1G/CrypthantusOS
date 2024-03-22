#pragma once

#include <stdint.h>

#define SECTOR_SIZE 512
#define MAX_PATH_SIZE 256
#define MAX_FILE_HANDLE 10
#define ROOT_DIR_HANDLE -1

typedef struct
{
    uint8_t id;
    uint16_t heads;
    uint16_t cylinders;
    uint16_t sectors;
} DISK;

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

typedef struct 
{
    int handle;
    int is_dir;
    uint32_t pos;
    uint32_t size;
} FAT_FILE;

typedef struct 
{
    uint8_t boot_jump_instruction[3];
    uint8_t oem_identifier[8];
    uint16_t sector_bytes;
    uint8_t cluster_sectors;
    uint16_t res_sectors;
    uint8_t fats;
    uint16_t dir_entries;
    uint16_t total_sectors;
    uint8_t media_desc_type;
    uint16_t fat_sectors;
    uint16_t track_sectors;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t large_sectors;

    //extended boot record
    uint8_t drive_num;
    uint8_t _reserved;
    uint8_t signature;
    uint32_t volume_id; //serial number
    uint8_t vol_label[11]; //padded with spaces
    uint8_t sys_id[8];
} __attribute__((packed)) FAT_BOOT_SECTOR;

typedef struct
{
    uint8_t buffer[SECTOR_SIZE];
    FAT_FILE file;
    int opened;
    uint32_t first_cluster;
    uint32_t cur_cluster;
    uint32_t cur_sector_in_cluster;
} FAT_FILE_DESC;

typedef struct
{
    union
    {
        FAT_BOOT_SECTOR boot_sector;
        uint8_t boot_sector_bytes[SECTOR_SIZE];
    } bs_t;

    FAT_FILE_DESC root_dir;
    FAT_FILE_DESC opened_files[MAX_FILE_HANDLE];
} FAT_DRIVER;

typedef struct
{
    FAT_DRIVER *driver;
    uint8_t *fat;
    uint32_t data_lba;
} FAT_DATA;

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

typedef struct {
    MEMORY_INFO memory;
    DISK disk;
    FAT_DATA fat_data;
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