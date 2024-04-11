#include <stdint.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdtype.h>
#include <boot/bootlib.h>
#include "fat.h"
#include <arch/i686/io.h>
#include <arch/i686/floppy.h>

/*enum FLOPPY_REGISTERS
{
   FLOPPY_REG_STATUS_A                  = 0x3F0, // read-only
   FLOPPY_REG_STATUS_B                  = 0x3F1, // read-only
   FLOPPY_REG_DIGITAL_OUT               = 0x3F2,
   FLOPPY_REG_TAPE_DRIVE                = 0x3F3,
   FLOPPY_REG_MAIN_STATUS               = 0x3F4, // read-only
   FLOPPY_REG_DATA_RATE_SELECT          = 0x3F4, // write-only
   FLOPPY_DATA_FIFO                     = 0x3F5,
   FLOPPY_REG_DIGITAL_IN                = 0x3F7, // read-only
   FLOPPY_CONF_CONTROl                  = 0x3F7 // write-only
};*/

static FAT_DRIVER *g_driver;
static uint8_t *g_fat = NULL;
static uint32_t g_data_lba; //data section

//remove disk if disk is not needed
/*int read_sectors(DISK* disk, uint32_t lba, uint8_t sectors, void *data)
{
    for (int i = 0; i < sectors; i++)
    {
        //set lba
        i686_outb(0x3F2, 1); //sector count
        i686_outb(0x3F3, lba);
        i686_outb(0x3F4, lba >> 8);
        i686_outb(0x3F5, lba >> 16);
        i686_outb(0x3F6, (lba >> 24) | 0xE0);

        //issue read sectors command
        i686_outb(0x3F7, 0x20);
        
        i686_insl(0x3F0, data, 128);
    }

    return 1;
}*/

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
        return 0;
    }

    FAT_FILE_DESC *file_desc = &g_driver->opened_files[handle];
    file_desc->file.handle = handle;
    file_desc->file.is_dir = (entry->attribute & FILE_ATTR_DIRECTORY) != 0;
    file_desc->file.pos = 0;
    file_desc->file.size = entry->size;
    file_desc->first_cluster = entry->first_cluster_low + ((uint32_t)entry->first_cluster_high << 16);
    file_desc->cur_cluster = file_desc->first_cluster;
    file_desc->cur_sector_in_cluster = 0;
    
    if (i686_floppy_read_sector(disk, FLOPPY_STANDARD_BASE, cluster_to_lba(file_desc->cur_cluster), file_desc->buffer))
    {
        printf("FAT Driver: read failure.\r\n");
        return 0;
    }

    file_desc->opened = 1;
    return &file_desc->file;
}

int fat_find_file(DISK *disk, FAT_FILE *file, const char *name, FAT_DIR_ENTRY *entry)
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
            return 1;
        }
    }

    return 0;
}

void fat_init(FAT_DATA* data)
{
    g_driver = data->driver;
    g_fat = data->fat;
    g_data_lba = data->data_lba;
}

FAT_FILE *fat_open(DISK *disk, const char *path)
{
    char name[MAX_PATH_SIZE];

    if (path[0] == '/')
        path++;
    
    FAT_FILE *current = &g_driver->root_dir.file;

    while (*path) {
        int is_last = 0;
        const char *delim = strchr(path, '/');

        if (delim != NULL)
        {
            memcpy(name, path, delim - path);
            name[delim - path] = '\0';
            path = delim + 1;
        }
        else
        {
            int len = strlen(path);
            memcpy(name, path, len);
            name[len] = '\0';
            path += len;
            is_last = 1;
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
                
                if (i686_floppy_read_sector(disk, FLOPPY_STANDARD_BASE, file_desc->cur_cluster, file_desc->buffer))
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

                if (i686_floppy_read_sector(disk, FLOPPY_STANDARD_BASE, cluster_to_lba(file_desc->cur_cluster) + file_desc->cur_sector_in_cluster, file_desc->buffer))
                {
                    printf("FAT Driver: error occurred while reading.\r\n");
                    break;
                }
            }
        }
    }

    return data_8t - (uint8_t*)data;
}

int fat_entry(DISK *disk, FAT_FILE *file, FAT_DIR_ENTRY *entry)
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
        g_driver->opened_files[file->handle].opened = 0;
    }
}