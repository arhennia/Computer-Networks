#include <stdio.h>

int main() {
    int num = 258;

    int byte1 = num & 255;
    int byte2 = (num >> 8) & 255;
    int byte3 = (num >> 16) & 255;
    int byte4 = (num >> 24) & 255;

    printf("Using Bitwise:\n");
    printf("Byte 1 = %d\n", byte1);
    printf("Byte 2 = %d\n", byte2);
    printf("Byte 3 = %d\n", byte3);
    printf("Byte 4 = %d\n", byte4);

    unsigned char *ptr = (unsigned char *)&num;

    printf("\nUsing Pointer:\n");
    printf("Byte 1 = %d\n", ptr[0]);
    printf("Byte 2 = %d\n", ptr[1]);
    printf("Byte 3 = %d\n", ptr[2]);
    printf("Byte 4 = %d\n", ptr[3]);

    if (byte1 == ptr[0])
        printf("\nLittle Endian\n");
    else
        printf("\nBig Endian\n");
    
    unsigned char temp;

    temp = ptr[0];
    ptr[0] = ptr[3];
    ptr[3] = temp;

    temp = ptr[1];
    ptr[1] = ptr[2];
    ptr[2] = temp;

    printf("\nAfter converting Little Endian to Big Endian:\n");
    printf("Number = %d\n", num);
    
    return 0;
}