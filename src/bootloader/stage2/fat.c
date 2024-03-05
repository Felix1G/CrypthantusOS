#include <stdint.h>
#include <stdbool.h>
#include "fat.h"
#include "string.h"
#include "stdio.h"
#include "stdtype.h"

#define SECTOR_SIZE 512
#define MAX_PATH_SIZE 256
#define MAX_FILE_HANDLE 10
#define ROOT_DIR_HANDLE -1


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
    bool opened;
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

static FAT_DRIVER *g_driver;
static uint8_t *g_fat = NULL;
static uint32_t g_data_lba; //data section

unsigned cluster_to_lba(unsigned cluster)
{
    return g_data_lba + (cluster - 2)  *g_driver->bs_t.boot_sector.cluster_sectors;
}

unsigned fat_next_cluster(unsigned current_cluster)
{
    unsigned idx = current_cluster  *3 / 2;

    if (current_cluster % 2 == 0)
        return (*(uint16_t*)(g_fat + idx)) & 0x0FFF;
    else
        return (*(uint16_t*)(g_fat + idx)) >> 4;
}


FAT_FILE *fat_open_entry(DISK *disk, FAT_DIR_ENTRY *entry)
{
    int handle = -1;
    for (int i = 0;i < MAX_FILE_HANDLE && handle < 0;i++)
    {
        if (!g_driver->opened_files[i].opened)
            handle = i;
    }

    if (handle < 0)
    {
        printf("FAT Driver: out of file handles.");
        return false;
    }

    FAT_FILE_DESC *file_desc = &g_driver->opened_files[handle];
    file_desc->file.handle = handle;
    file_desc->file.is_dir = (entry->attribute & FILE_ATTR_DIRECTORY) != 0;
    file_desc->file.pos = 0;
    file_desc->file.size = entry->size;
    file_desc->first_cluster = entry->first_cluster_low + ((uint32_t)entry->first_cluster_high << 16);
    file_desc->cur_cluster = file_desc->first_cluster;
    file_desc->cur_sector_in_cluster = 0;
    
    if (!disk_read_sectors(disk, cluster_to_lba(file_desc->cur_cluster), 1, file_desc->buffer))
    {
        printf("FAT Driver: read failure.\r\n");
        return false;
    }

    file_desc->opened = true;
    return &file_desc->file;
}

bool fat_find_file(DISK *disk, FAT_FILE *file, const char *name, FAT_DIR_ENTRY *entry)
{
    char fat_name[12];

    //convert name to fat name
    memset(fat_name, ' ', sizeof(name));
    fat_name[11] = '\0';

    const char *ext = strchr(name, '.');
    
    int name_end = 0;
    for (name_end;name_end < 8 && name[name_end] && (ext == 0 || name + name_end < ext); name_end++)
    {
        fat_name[name_end] = toupper(name[name_end]);
    }

    int max_len = 12;
    if (ext != NULL)
    {
        for (int i = 0;i < 3 && ext[i + 1];i++)
            fat_name[i + 8] = toupper(ext[i + 1]);
        max_len = 8;
    }

    for (int i = name_end;i < max_len;i++)
        fat_name[i] = ' ';
    
    FAT_DIR_ENTRY file_entry;
    while (fat_entry(disk, file, &file_entry))
    {
        if (!memcmp(fat_name, file_entry.name, 11))
        {
            *entry = file_entry;
            return true;
        }
    }

    return false;
}

int fat_init(DISK *disk)
{
    uint32_t fat_size;
    uint32_t root_dir_lba;
    uint32_t root_dir_size;
    int index;

    g_driver = (FAT_DRIVER*) MEMORY_FAT_ADDR;
    
    if (!disk_read_sectors(disk, 0, 1, g_driver->bs_t.boot_sector_bytes))
    {
        printf("FAT Driver: read boot sector failed.\r\n");
        return false;
    }

    g_fat = (uint8_t*)g_driver + sizeof(FAT_DRIVER);
    fat_size = g_driver->bs_t.boot_sector.sector_bytes  *g_driver->bs_t.boot_sector.fat_sectors;
    
    if (sizeof(FAT_DRIVER) + fat_size >= MEMORY_FAT_SIZE)
    {
        printf("FAT Driver: not enough memory to read FAT. Required memory: %lu, but only have %u\r\n", 
            sizeof(FAT_DRIVER) + fat_size, MEMORY_FAT_SIZE);
        return false;
    }

    if (!disk_read_sectors(disk, g_driver->bs_t.boot_sector.res_sectors, g_driver->bs_t.boot_sector.fat_sectors, g_fat))
    {
        printf("FAT Driver: failed to read FAT.\r\n");
        return false;
    }

    root_dir_lba = g_driver->bs_t.boot_sector.res_sectors + g_driver->bs_t.boot_sector.fat_sectors  *g_driver->bs_t.boot_sector.fats;
    root_dir_size = sizeof(FAT_DIR_ENTRY)  *g_driver->bs_t.boot_sector.dir_entries;

    g_driver->root_dir.file.handle = ROOT_DIR_HANDLE;
    g_driver->root_dir.file.is_dir = true;
    g_driver->root_dir.file.pos = 0;
    g_driver->root_dir.file.size = root_dir_size;
    g_driver->root_dir.opened = true;
    g_driver->root_dir.first_cluster = root_dir_lba;
    g_driver->root_dir.cur_cluster = root_dir_lba;
    g_driver->root_dir.cur_sector_in_cluster = 0;

    if (!disk_read_sectors(disk, root_dir_lba, 1, g_driver->root_dir.buffer))
    {
        printf("FAT Driver: failed to read root directory.\r\n");
        return false;
    }

    g_data_lba = root_dir_lba + 
            (root_dir_size + g_driver->bs_t.boot_sector.sector_bytes - 1) / 
                    g_driver->bs_t.boot_sector.sector_bytes;

    //reset opened files
    for (index = 0; index < MAX_FILE_HANDLE; index++)
        g_driver->opened_files[index].opened = false;

    return true;
}

FAT_FILE *fat_open(DISK *disk, const char *path)
{
    char name[MAX_PATH_SIZE];

    if (path[0] == '/')
        path++;
    
    FAT_FILE *current = &g_driver->root_dir.file;

    while (*path) {
        bool is_last = false;
        const char *delim = strchr(path, '/');

        if (delim != NULL)
        {
            memcpy(name, path, delim - path);
            name[delim - path + 1] = '\0';
            path = delim + 1;
        }
        else
        {
            int len = strlen(path);
            memcpy(name, path, len);
            name[len + 1] = '\0';
            path += len;
            is_last = true;
        }
        
        FAT_DIR_ENTRY entry;
        if (fat_find_file(disk, current, name, &entry))
        {
            fat_close(current);

            if (!is_last && (entry.attribute & FILE_ATTR_DIRECTORY == 0))
            {
                printf("FAT Driver: %s is not a directory\r\n", name);
                return NULL;
            }

            current = fat_open_entry(disk, &entry);
        }
        else
        {
            fat_close(current);

            printf("FAT Driver: %s not found.\r\n", name);
            return NULL;
        }
    }

    return current;
}

unsigned fat_read(DISK *disk, FAT_FILE *file, unsigned n, void *data)
{
    FAT_FILE_DESC *file_desc = 
        (file->handle == ROOT_DIR_HANDLE) ? 
            &g_driver->root_dir : 
            &g_driver->opened_files[file->handle];

    uint8_t *data_8t = (uint8_t*)data;

    if (!file_desc->file.is_dir || file_desc->file.size != 0)
        n = min(n, file_desc->file.size - file_desc->file.pos);

    while (n > 0)
    {
        uint32_t relative_pos = file_desc->file.pos % SECTOR_SIZE;
        uint32_t left = SECTOR_SIZE - relative_pos;
        uint32_t copy_size = min(n, left);
        
        memcpy(data_8t, file_desc->buffer + relative_pos, copy_size);
        data_8t += copy_size;
        file_desc->file.pos += copy_size;
        n -= copy_size;

        //check for more data to read
        if (left == copy_size)
        {
            if (file_desc->file.handle == ROOT_DIR_HANDLE)
            {
                file_desc->cur_cluster++;
                
                if (!disk_read_sectors(disk, file_desc->cur_cluster, 1, file_desc->buffer))
                {
                    printf("FAT Driver: failed to read root directory.\r\n");
                    break;
                }
            }
            else
            {
                if (++file_desc->cur_sector_in_cluster >= g_driver->bs_t.boot_sector.cluster_sectors)
                {
                    file_desc->cur_sector_in_cluster = 0;
                    file_desc->cur_cluster = fat_next_cluster(file_desc->cur_cluster);
                }

                //EOF
                if (file_desc->cur_cluster >= 0xFF8)
                {
                    file_desc->file.size = file_desc->file.pos;
                    break;
                }

                if (!disk_read_sectors(disk, cluster_to_lba(file_desc->cur_cluster) + file_desc->cur_sector_in_cluster, 1, file_desc->buffer))
                {
                    printf("FAT Driver: error occurred while reading.\r\n");
                    break;
                }
            }
        }
    }

    return data_8t - (uint8_t*)data;
}

bool fat_entry(DISK *disk, FAT_FILE *file, FAT_DIR_ENTRY *entry)
{
    return fat_read(disk, file, sizeof(FAT_DIR_ENTRY), entry) == sizeof(FAT_DIR_ENTRY);
}

void fat_close(FAT_FILE *file)
{
    if (file->handle == ROOT_DIR_HANDLE)
    {
        file->pos = 0;
        g_driver->root_dir.cur_cluster = g_driver->root_dir.first_cluster;
    }
    else
    {
        g_driver->opened_files[file->handle].opened = false;
    }
}