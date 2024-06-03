#include <stdio.h>
#include <stdlib.h>

int main(void) {

    int i;

    for (i = 0; i < 0x19; i = i + 1) {
        printf("%d %d\n", i, 0x32 - i + -1);
    }

    // for (i = 0; i < 0x32; i = i + 1) {
    // //   cVar1 = local_88[i];
    // //   local_88[i] = local_88[i + 1];
    // //   local_88[i + 1] = cVar1;
    // }

    // local_88[49] = 0;

    // for (i = 0; i < 0x30; i = i + 2) {
    //     cVar1 = local_88[i];
    //     local_88[i] = local_88[i + 1];
    //     local_88[i + 1] = cVar1;
    // }

    unsigned long long local_48 = 0x6a20756f6e727965;
    unsigned long long local_40 = 0x72636361696b676e;
    unsigned long long local_38 = 0x7275732066612065;
    unsigned long long local_30 = 0x7473726120746f79;
    unsigned long long local_28 = 0x616579647420206f;
    unsigned long long local_20 = 0x756f612065727220;
    unsigned long long local_18 = 0x0000000000000059;
    long local_10;

    printf("%s\n", (char*) &local_48);

}

