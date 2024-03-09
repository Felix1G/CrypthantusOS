#pragma once

void buddy_init();
void *buddy_alloc(unsigned size);
void buddy_free(void *ptr);