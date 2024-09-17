#include <stdio.h>

int main()
{
    char *dst;
    unsigned int color_code = 0xFF0000;  // Red color code (in hex)
    *(unsigned int *)dst = color_code;

    printf("Color code: 0x%X\n", *dst);
}