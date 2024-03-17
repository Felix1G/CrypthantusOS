#include "stdtype.h"

int isupper(int x)
{
    return x >= 'A' && x <= 'Z';
}

int islower(int x)
{
    return x >= 'a' && x <= 'z';
}

int toupper(int ch)
{
    return islower(ch) ? ch - 32 : ch;
}

int tolower(int ch)
{
    return isupper(ch) ? ch + 32 : ch;
}
