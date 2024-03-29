#include <stdint.h>
#include <stdbool.h>
#include "fat.h"
#include "string.h"
#include "stdio.h"
#include "stdtype.h"

static FAT_DRIVER *driver;
static uint8_t *fat = NULL;
static uint32_t data_lba; //data section

void fat_export_data(FAT_DATA* data)
{
    data->driver = driver;
    data->fat = fat;
    data->data_lba = data_lba;
}

unsigned cluster_to_lba(unsigned cluster)
{
    return data_lba + (cluster - 2)  *driver->bs_t.boot_sector.cluster_sectors;
}

unsigned fat_next_cluster(unsigned current_cluster)
{
    unsigned idx = current_cluster  *3 / 2;

    if (current_cluster % 2 == 0)
        return (*(uint16_t*)(fat + idx)) & 0x0FFF;
    else
        return (*(uint16_t*)(fat + idx)) >> 4;
}


FAT_FILE *fat_open_entry(DISK *disk, FAT_DIR_ENTRY *entry)
{
    int handle = -1;
    for (int i = 0;i < MAX_FILE_HANDLE && handle < 0;i++)
    {
        if (!driver->opened_files[i].opened)
            handle = i;
    }

    if (handle < 0)
    {
        printf("FAT Driver: out of file handles.");
        return false;
    }

    FAT_FILE_DESC *file_desc = &driver->opened_files[handle];
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

    driver = (FAT_DRIVER*) MEMORY_FAT_ADDR;
    
    if (!disk_read_sectors(disk, 0, 1, driver->bs_t.boot_sector_bytes))
    {
        printf("FAT Driver: read boot sector failed.\r\n");
        return false;
    }

    fat = (uint8_t*)driver + sizeof(FAT_DRIVER);
    fat_size = driver->bs_t.boot_sector.sector_bytes  *driver->bs_t.boot_sector.fat_sectors;
    
    if (sizeof(FAT_DRIVER) + fat_size >= MEMORY_FAT_SIZE)
    {
        printf("FAT Driver: not enough memory to read FAT. Required memory: %lu, but only have %u\r\n", 
            sizeof(FAT_DRIVER) + fat_size, MEMORY_FAT_SIZE);
        return false;
    }

    if (!disk_read_sectors(disk, driver->bs_t.boot_sector.res_sectors, driver->bs_t.boot_sector.fat_sectors, fat))
    {
        printf("FAT Driver: failed to read FAT.\r\n");
        return false;
    }

    root_dir_lba = driver->bs_t.boot_sector.res_sectors + driver->bs_t.boot_sector.fat_sectors  *driver->bs_t.boot_sector.fats;
    root_dir_size = sizeof(FAT_DIR_ENTRY)  *driver->bs_t.boot_sector.dir_entries;

    driver->root_dir.file.handle = ROOT_DIR_HANDLE;
    driver->root_dir.file.is_dir = true;
    driver->root_dir.file.pos = 0;
    driver->root_dir.file.size = root_dir_size;
    driver->root_dir.opened = true;
    driver->root_dir.first_cluster = root_dir_lba;
    driver->root_dir.cur_cluster = root_dir_lba;
    driver->root_dir.cur_sector_in_cluster = 0;

    if (!disk_read_sectors(disk, root_dir_lba, 1, driver->root_dir.buffer))
    {
        printf("FAT Driver: failed to read root directory.\r\n");
        return false;
    }

    data_lba = root_dir_lba + 
            (root_dir_size + driver->bs_t.boot_sector.sector_bytes - 1) / 
                    driver->bs_t.boot_sector.sector_bytes;

    //reset opened files
    for (index = 0; index < MAX_FILE_HANDLE; index++)
        driver->opened_files[index].opened = false;

    return true;
}

FAT_FILE *fat_open(DISK *disk, const char *path)
{
    char name[MAX_PATH_SIZE];

    if (path[0] == '/')
        path++;
    
    FAT_FILE *current = &driver->root_dir.file;

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
            name[len] = '\0';
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
            &driver->root_dir : 
            &driver->opened_files[file->handle];

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
                if (++file_desc->cur_sector_in_cluster >= driver->bs_t.boot_sector.cluster_sectors)
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
        driver->root_dir.cur_cluster = driver->root_dir.first_cluster;
    }
    else
    {
        driver->opened_files[file->handle].opened = false;
    }
}