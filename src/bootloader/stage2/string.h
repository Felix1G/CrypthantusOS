#pragma once

char *strchr(const char *str, int c);
char *strcpy(char *destination, const char *source);
unsigned strlen(const char *str);
void *memcpy(void *dest, const void *src, unsigned n);
void *memset(void *str, int c, unsigned n);
int memcmp(const void *str1, const void *str2, unsigned n);