#pragma once

#include <boot/bootlib.h>

void cmd_echo(const char* arg);
int cmd_shutdown();
void cmd_memmap(BOOT_DATA* boot_data);