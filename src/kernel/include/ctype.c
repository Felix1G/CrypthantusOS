#include "ctype.h"

int isalpha(int argument)
{
    return (argument >= 'a' && argument <= 'z') || (argument >= 'A' && argument <= 'Z');
}

int isdigit(int argument)
{
    return argument >= '0' && argument <= '9';
}

int isalnum(int argument)
{
    return isalpha(argument) || isdigit(argument);
}