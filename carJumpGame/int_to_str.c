#include <stdio.h>

// turn integer to string
void int_to_str(char str[], int num, int totalLength)
{
    int i;
    int remaining;
    int len = 0;
    int n;
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    if (len < totalLength)
        len = totalLength;
    for (i = 0; i < len; i++)
    {
        remaining = num % 10;
        num = num / 10;
        str[len - (i + 1)] = remaining + '0';
    }
    str[len] = '\0';
}
