#include <stdint.h>
#include "stdio.h"
#include "x86.h"

#define PRINTF_STATE_PRINT 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_SPECIFIER 2

const char* g_number_chars_lowercase = "0123456789abcdefg";
const char* g_number_chars_uppercase = "0123456789ABCDEFG";

const unsigned SCREEN_WIDTH = 80;
const unsigned SCREEN_HEIGHT = 25;
const uint8_t DEFAULT_COLOUR = 0x7;

uint8_t* g_scr_buff = (uint8_t*)0xB8000;
int g_scr_x = 0, g_scr_y = 0;

void screen_pos(int *scr_x, int *scr_y)
{
    *scr_x = g_scr_x;
    *scr_y = g_scr_y;
}

void putchar(int x, int y, char c)
{
    g_scr_buff[2 * (y * SCREEN_WIDTH + x)] = c;
}

void putclr(int x, int y, uint8_t colour)
{
    g_scr_buff[2 * (y * SCREEN_WIDTH + x) + 1] = colour;
}

char getc(int x, int y)
{
    return g_scr_buff[2 * (y * SCREEN_WIDTH + x)];
}

uint8_t getclr(int x, int y)
{
    return g_scr_buff[2 * (y * SCREEN_WIDTH + x) + 1];
}

void setcrsr(int x, int y) //cursor
{
    int pos = y * SCREEN_WIDTH + x;
    
    x86_outb(0x3D4, 0x0F);
    x86_outb(0x3D5, (uint8_t)(pos & 0xFF));
    x86_outb(0x3D4, 0x0E);
    x86_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void scrollback(int lines)
{
    for (int y = lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_HEIGHT; x++)
        {
            putchar(x, y - lines, getc(x, y));
            putclr(x, y - lines, getclr(x, y));
        }

    for (int y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_HEIGHT; x++)
        {
            putchar(x, y, '\0');
            putclr(x, y, DEFAULT_COLOUR);
        }

    g_scr_y -= lines;
}

void clrscr()
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            putchar(x, y, '\0');
            putclr(x, y, DEFAULT_COLOUR);
        }
    }

    g_scr_x = 0;
    g_scr_y = 0;

    setcrsr(g_scr_x, g_scr_y);
}

void putc(char c)
{
    switch (c)
    {
        case '\n':
            g_scr_x = 0;
            g_scr_y++;
            break;

        case '\t':
            int rel_g_scr_x = g_scr_x % 4;
            for (int i = 0;i < 4 - rel_g_scr_x;i++)
                putc(' ');
            break;

        case '\r':
            g_scr_x = 0;
            break;

        default:
            putchar(g_scr_x, g_scr_y, c);
            g_scr_x++;
            break;
    }

    if (g_scr_x > SCREEN_WIDTH)
    {
        g_scr_x = 0;
        g_scr_y++;
    }

    if (g_scr_y >= SCREEN_HEIGHT)
    {
        scrollback(1);
    }

    setcrsr(g_scr_x, g_scr_y);
}

void puts(const char* str)
{
    while (*str)
    {
        putc(*str);
        str++;
    }
}

void printf_number(void* argp, int sign, int radix, int uppercase, int long_t)
{
    char buffer[32];

    const char* number_chars = uppercase ? g_number_chars_uppercase : g_number_chars_lowercase;

    int negative = 0;
    uint64_t number;
    if (long_t) 
    {
        if (sign)
        {
            long int val = *(long int*)argp;
            if (val < 0)
            {
                val = -val;
                negative = 1;
            }
            number = (unsigned long int)val;
        }
        else
        {
            number = *(unsigned long int*)argp;
        }
    } else {
        if (sign)
        {
            int val = *(int*)argp;
            if (val < 0)
            {
                val = -val;
                negative = 1;
            }
            number = val;
        }
        else
        {
            number = *(unsigned int*)argp;
        }
    }

    int8_t pos = 0;
    do {
        uint64_t quotient = number / radix;
        uint64_t remainder = number % radix;
        buffer[pos++] = number_chars[remainder];
        number = quotient;
    } while (number > 0);

    if (negative)
        buffer[pos++] = '-';

    while (pos-- > 0)
    {
        putc(buffer[pos]);
    }
}

void printf(const char* format, ...)
{
    int* argp = ((int*)&format) + 1;
    int state = PRINTF_STATE_PRINT;
    int len = 0;

    while (*format)
    {
        char c = *format;
        
        switch (state)
        {
        case PRINTF_STATE_PRINT:
            switch (c)
            {
                case '%':
                    state = PRINTF_STATE_SPECIFIER;
                    break;
                default:
                    putc(c);
            }
            break;
        case PRINTF_STATE_LENGTH:
            len = 1;
        case PRINTF_STATE_SPECIFIER:
            state = PRINTF_STATE_PRINT;
            if (len)
                argp--;
            
            switch (c)
            {
                case 'd':
                case 'i':
                    printf_number(argp, 1, 10, 1, len);
                    break;
                case 'u':
                    printf_number(argp, 0, 10, 1, len);
                    break;
                case 'X':
                    printf_number(argp, 0, 16, 1, len);
                    break;
                case 'x':
                    printf_number(argp, 0, 16, 0, len);
                    break;
                case 'o':
                    printf_number(argp, 0, 8, 1, len);
                    break;
                case 'l':
                    state = PRINTF_STATE_LENGTH;
                    break;
                case 's':
                    puts((const char*)*argp);
                    break;
                case 'c':
                    putc((char)*argp);
                    break;
                case '%':
                    putc('%');
                    state = PRINTF_STATE_PRINT;
                    break;
            }

            argp++;
            if (len)
            {
                argp++;
                len = 0;
            }

            break;
        }

        format++;
    }
}


void print_buffer(const char* msg, const void* buffer, unsigned n)
{
    const uint8_t* buffer_8t = (const uint8_t*) buffer;
    
    puts(msg);
    
    for (uint16_t i = 0;i < n;i++)
    {
        putc(g_number_chars_uppercase[buffer_8t[i] >> 4]);
        putc(g_number_chars_uppercase[buffer_8t[i] & 0xF]);
    }
    
    putc('\n');
}
