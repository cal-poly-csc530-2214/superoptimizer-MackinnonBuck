#include <stdio.h>

int main()
{
    int a, b;
    scanf("%d", &a);

    asm(
        "mov        %0, %%ebx;"
        "shr        $31, %%ebx;"
        "mov        $0x80000000, %%ecx;"
        "sub        %%ebx, %%ecx;"
        "mov        %%ecx, %%edx;"
        "xor        %0, %%edx;"
        "mov        %%edx, %1;"
        "sub        %%ecx, %1;"
        : "=r" (b)
        : "r" (a));

    printf("%d\n", b);
}
