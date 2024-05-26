/**
This program generates the xor values and swap indices using the random seed they provided.
These values will be used in the python script to generate the solution.
*/

#include <stdio.h>
#include <stdlib.h>

int main(void) {

    int i;
    int seedStr = 0x65766f4c;
    long seed = strtol((char *)&seedStr,(char **)0x0,10);
    srand((uint)seed);

    printf("**** XOR (index, value) ****\n");
    for (i = 0; i < 0x1a; i++) {
        int r = rand();
        printf("(%d, 0x%x),", i, (unsigned char)r);
    }

    printf("\n\n**** Swap Indices ****\n");
    for (i = 0x19; i > 0; i--) {
        int r = rand();
        int swap_index = r % (i + 1);
        printf("(0x%x, 0x%x),", i, swap_index);
    }

}