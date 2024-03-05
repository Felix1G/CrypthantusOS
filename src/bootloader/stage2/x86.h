#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <boot/bootlib.h>

#define ASMCALL __attribute__((cdecl))

void ASMCALL x86_outb(uint16_t port, uint8_t value);
uint8_t ASMCALL x86_inb(uint16_t port);

bool ASMCALL x86_disk_reset(int8_t disk);
bool ASMCALL x86_disk_read(int8_t disk, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t count, void *output);
bool ASMCALL x86_disk_drive_params(int8_t disk, uint8_t *drive_type, uint16_t *sectors, uint16_t *cylinders, uint16_t *heads);

int ASMCALL x86_e820_next_block(MEMORY_BLOCK* block, uint32_t* continue_id);