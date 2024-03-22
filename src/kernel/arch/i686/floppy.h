#pragma once

#include <stdint.h>
#include <boot/bootlib.h>

#define FLOPPY_STANDARD_BASE    0x03f0
#define FLOPPY_DIR_READ         0x01
#define FLOPPY_DIR_WRITE        0x02

void i686_detect_drives();

//returns 1 if error, 0 if success
int i686_floppy_reset(int base);
//returns 1 if error, 0 if success
int floppy_dma_init(int dir);
//returns 2 if fatal error, 1 if failed retries error, 0 if success
int i686_floppy_read_sector(DISK* disk, int base, int lba, void* data);
//returns 2 if fatal error, 1 if failed retries error, 0 if success
int i686_floppy_writer_sector(DISK* disk, int base, int lba, void* data);