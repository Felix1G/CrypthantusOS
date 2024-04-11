#include <stdint.h>
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "hal/hal.h"

#define PRINTF_STATE_PRINT 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_SPECIFIER 2
#define PRINTF_STATE_DIGIT_SPECIFIER 3

const char* g_number_chars_lowercase = "0123456789abcdefg";
const char* g_number_chars_uppercase = "0123456789ABCDEFG";

const unsigned SCREEN_WIDTH = 80;
const unsigned SCREEN_HEIGHT = 25;
const uint8_t DEFAULT_COLOUR = 0x7;

uint8_t* g_scr_buff = (uint8_t*)0xB8000;
int g_scr_x = 0, g_scr_y = 0;
int out_scr_x = 0, out_scr_y = 0;

unsigned char_written = 0;

void set_screen_pos(int scr_x, int scr_y)
{
    g_scr_x = scr_x;
    g_scr_y = scr_y;
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
    
    hal_outb(0x3D4, 0x0F);
    hal_outb(0x3D5, (uint8_t)(pos & 0xFF));
    hal_outb(0x3D4, 0x0E);
    hal_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void scrollback(int lines)
{
    for (int y = lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            putchar(x, y - lines, getc(x, y));
            putclr(x, y - lines, getclr(x, y));
        }

    for (int y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            putchar(x, y, '\0');
            putclr(x, y, DEFAULT_COLOUR);
        }

    g_scr_y -= lines;
    out_scr_y -= lines;

    if (g_scr_y < 0)
    {
        g_scr_y = 0;
        g_scr_x = 0;
    }
    
    if (out_scr_y < 0)
    {
        out_scr_y = 0;
        out_scr_x = 0;
    }
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

void putc(int c)
{
    char_written++;

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

        case KEY_BACKSPACE:
            if ((g_scr_y > 0 || g_scr_x > 0) && (g_scr_y > out_scr_y || (g_scr_y == out_scr_y && g_scr_x > out_scr_x))) {
                g_scr_x--;

                //backspace to the previous line
                if (g_scr_x < 0) {
                    g_scr_x = SCREEN_WIDTH - 1;
                    g_scr_y--;
                }

                //if backspace is between non-zero characters, replace with space instead
                for (int i = g_scr_x + 1; i < SCREEN_WIDTH; i++)
                    if (getc(i, g_scr_y) != '\0') {
                        putchar(g_scr_x, g_scr_y, ' ');
                        goto putc_backspace_done;
                    }
                putchar(g_scr_x, g_scr_y, '\0');
            }
putc_backspace_done:
            break;

        case KEY_UP:
            if (g_scr_y > 0)
                g_scr_y--;
            break;

        case KEY_DOWN:
            if (g_scr_y < out_scr_y)
                g_scr_y++;
            break;

        case KEY_LEFT:
            if (g_scr_y > 0 || g_scr_x > 0)
                g_scr_x--;
            break;

        case KEY_RIGHT:
            if (getc(g_scr_x, g_scr_y) == '\0')
            {
                if (g_scr_y < out_scr_y)
                {
                    g_scr_y++;
                    g_scr_x = 0;
                }
            } else
                g_scr_x++;
            break;

        default:
            if (g_scr_y > out_scr_y || (g_scr_y == out_scr_y && g_scr_x >= out_scr_x))
            {
                putchar(g_scr_x, g_scr_y, c);
                g_scr_x++;
                goto putc_skip_line_check;
            }
            break;
    }

    if (g_scr_x < 0)
    {
        g_scr_y--;
        g_scr_y = max(g_scr_y, 0);
        g_scr_x = SCREEN_WIDTH - 1;
    }

    if (getc(g_scr_x, g_scr_y) == 0x00) {
        for (int i = g_scr_x - 1;i >= 0;i--)
        {
            if (getc(i, g_scr_y))
            {
                g_scr_x = i + 1;
                goto putc_found;
            }
        }
        g_scr_x = 0;
        putc_found: ;
    }

putc_skip_line_check:

    if (g_scr_x >= SCREEN_WIDTH)
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

void printf_number(void* argp, int sign, int radix, int uppercase, int long_t, int min_digit)
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

        min_digit--;
    } while (number > 0);

    if (negative)
        buffer[pos++] = '-';

    for (int i = 0;i < min_digit;i++)
        putc('0');

    while (pos-- > 0)
    {
        putc(buffer[pos]);
    }
}

bool g_enable_echo = true;
void io_enable_echo(int en) { g_enable_echo = !!en; } //1 or 0
int io_echo() { return g_enable_echo; }

int writef(const char* fmt, int* argp)
{
    int log = !memcmp(fmt, "[LOG]", 5);
    int debug = !memcmp(fmt, "[DEBUG]", 7);
    int error = !memcmp(fmt, "[ERROR]", 7);

    if (log)
        fmt += 5;
    else if (debug | error)
        fmt += 7;

    if (g_enable_echo) {
        const char* log_prefix;
        if (log) log_prefix = "(KERNEL): ";
        else if (debug) log_prefix = "(KERNEL) DEBUG: ";
        else if (error) log_prefix = "(KERNEL) ERROR: ";
        else goto printf_begin;

        while (*log_prefix) {
            putc(*log_prefix);
            log_prefix++;
        }
    }

printf_begin:
    int state = PRINTF_STATE_PRINT;
    int len = 0;

    char_written = 0;

    int min_digit = 0;
    
    while (*fmt)
    {
        char c = *fmt;
        
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
        case PRINTF_STATE_DIGIT_SPECIFIER:
        case PRINTF_STATE_SPECIFIER:
            bool digit_state = state == PRINTF_STATE_DIGIT_SPECIFIER;
            state = PRINTF_STATE_PRINT;
            if (len)
                argp--;
            
            switch (c)
            {
                case 'd':
                case 'i':
                    printf_number(argp, 1, 10, 1, len, min_digit);
                    break;
                case 'u':
                    printf_number(argp, 0, 10, 1, len, min_digit);
                    break;
                case 'X':
                    printf_number(argp, 0, 16, 1, len, min_digit);
                    break;
                case 'x':
                    printf_number(argp, 0, 16, 0, len, min_digit);
                    break;
                case 'o':
                    printf_number(argp, 0, 8, 1, len, min_digit);
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
                case '0':
                    if (digit_state)
                        min_digit *= 10;
                    state = PRINTF_STATE_DIGIT_SPECIFIER;
                    goto printf_digit_continue_while_loop;
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    min_digit *= 10;
                    min_digit = c - '0';

                    state = PRINTF_STATE_DIGIT_SPECIFIER;
                    goto printf_digit_continue_while_loop;
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

        min_digit = 0;

printf_digit_continue_while_loop:
        fmt++;
    }

    if (log | debug | error)
        putc('\n');

    if (g_scr_y > out_scr_y) {
        out_scr_x = g_scr_x;
        out_scr_y = g_scr_y;
    } else
        out_scr_x = max(out_scr_x, g_scr_x);

    out_scr_x = max(out_scr_x, 0);
    out_scr_y = max(out_scr_y, 0);

    return char_written;
}

int _read_console(const char* fmt, int* argp, int argcount, char* _buffer)
{
    char* buffer = _buffer;
    buffer[0] = '\0';

    //wait until an enter is pressed, then read the input
    while (true) {
        if (hal_wait_keyboard_interrupt() && !hal_keyboard_up() && hal_keyboard_char() == '\n') {
            break;
        }
    }
    
    //copy the input from the console to the buffer
    int buffer_index = 0;
    int x = out_scr_x;
    for (int y = out_scr_y; y < g_scr_y; y++) {
        for (x; x < SCREEN_WIDTH; x++)
        {
            register int c = getc(x, y);
            buffer[buffer_index] = c;
            if (c == '\0')
                break;
            buffer_index++;
        }
        x = 0;
    }
    buffer[buffer_index] = '\0';
    
    //time to read the input!
    while (*fmt && *buffer)
    {
        switch (*fmt)
        {
            case ' ':
                break;
            case '%':
                fmt++;
                switch (*fmt)
                {
                    case 's':
                        char* buffer_end = buffer + buffer_index + 1;

                        //skip whitespaces until the first non-whitespace character is found
                        while (*buffer)
                            if (*(buffer++) != ' ')
                                break;
                        buffer--;

                        memcpy(*((char**)argp), buffer, buffer_end - buffer);
                        argp++;
                        argcount++;
                        break;
                        
                    case 'd':
                    case 'u':
                        bool found_number = false;
                        bool negative = false;
                        bool positive = false;
                        uint64_t number = 0;
                        while (*buffer)
                        {
                            if (*buffer == ' ')
                            {
                                if (found_number) break;
                            }
                            else if (*buffer != ' ')
                            {
                                if (!negative && *buffer == '-')
                                {
                                    negative = true;
                                    buffer++;
                                    continue;
                                }

                                if (!positive && *buffer == '+')
                                {
                                    positive = true;
                                    buffer++;
                                    continue;
                                }

                                if (isdigit(*buffer))
                                {
                                    number *= 10;
                                    number += *buffer - '0';
                                }
                                else
                                {
                                    break;
                                }
                                found_number = true;
                            }
                            
                            buffer++;
                        }
                        if (negative) number = -number;
                        **((int**)argp) = (int)number;
                        argp++;
                        argcount++;
                        break;
                }
            break;
        }
        
        fmt++;
    }
    
    //update the output stream cursor
    out_scr_x = g_scr_x;
    out_scr_y = g_scr_y;
    if (out_scr_x < 0) {
        out_scr_x = SCREEN_WIDTH - 1;
        out_scr_y--;
    }
    
    if (*fmt)
        _read_console(fmt, argp, argcount, _buffer);

    return argcount;
}

int readf(const char* fmt, int* argp)
{
    char _buffer[SCREEN_WIDTH * SCREEN_HEIGHT + 1];
    return _read_console(fmt, argp, 0, _buffer);
}

//should not be called by user inputs to prevent mismatch of the console output-only cursor
int printf(const char* fmt, ...)
{
    return writef(fmt, ((int*)&fmt) + 1);
}

int scanf(const char* fmt, ...)
{
    return readf(fmt, ((int*)&fmt) + 1);
}