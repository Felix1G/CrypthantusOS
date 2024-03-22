#pragma once

#include <stdint.h>
#include <boot/bootlib.h>
#include "disk.h"

int fat_init(DISK *disk);
FAT_FILE *fat_open(DISK *disk, const char *path);
unsigned fat_read(DISK *disk, FAT_FILE *file, unsigned n, void *data);
bool fat_entry(DISK *disk, FAT_FILE *file, FAT_DIR_ENTRY *entry);
void fat_close(FAT_FILE *file);
void fat_export_data(FAT_DATA* data);