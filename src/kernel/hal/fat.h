#pragma once

#include <stdint.h>
#include <boot/bootlib.h>

void fat_init(FAT_DATA *data);
FAT_FILE *fat_open(DISK *disk, const char *path);
unsigned fat_read(DISK *disk, FAT_FILE *file, unsigned n, void *data);
int fat_entry(DISK *disk, FAT_FILE *file, FAT_DIR_ENTRY *entry);
void fat_close(FAT_FILE *file);