#include <stdint.h>
#include "string.h"
#include "stdio.h"

char *strchr(const char *str, int c)
{
    if (str == NULL)
        return NULL;

    while (*str)
    {
        if (*str == c) {
            char* c = (char*)str; //removes const
            return c;
        }
        
        str++;
    }

    return NULL;
}

char *strcpy(char *destination, const char *source)
{
    char *dst_first = destination;

    if (destination == NULL)
        return NULL;

    if (source == NULL)
    {
        *destination = '\0';
        return destination;
    }

    while (*source)
    {
        *destination = *source;
        source++;
        destination++;
    }

    *destination = '\0';
    return dst_first;
}

unsigned strlen(const char *str)
{
    unsigned len = 0;
    
    while (*str)
    {
        len++;
        str++;
    }

    return len;
}

void *memcpy(void *dest, const void *src, unsigned n)
{
    uint8_t *dest_8t = (uint8_t*)dest;
    const uint8_t *src_8t = (const uint8_t*)src;
    
    for (int i = 0;i < n;i++)
        dest_8t[i] = src_8t[i];
    
    return dest;
}

void *memset(void *str, int c, unsigned n)
{
    uint8_t *str_8t = (uint8_t*)str;

    for (int i = 0;i < n;i++)
        str_8t[i] = (uint8_t)c;

    return str;
}

int memcmp(const void *str1, const void *str2, unsigned n)
{
    const uint8_t *str1_8t = (const uint8_t*)str1;
    const uint8_t *str2_8t = (const uint8_t*)str2;

    for (int i = 0;i < n;i++) {
        if (str1_8t[i] != str2_8t[i])
            return (str1_8t[i] < str2_8t[i]) ? -1 : 1;
    }
        
    return 0;
}

int strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
            return (*str1 < *str2) ? -1 : 1;
        
        str1++;
        str2++;
    }

    if (*str1 || *str2)
        return (*str1 < *str2) ? -1 : 1;

    return 0;
}