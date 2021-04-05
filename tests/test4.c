#include <stdio.h>

int main()
{
    int a, b;
    scanf("%d", &a);

    asm(
        "mov        %0, %%ebx;"
        "shr        $31, %%ebx;"
        "mov        %%ebx, %%ecx;"
        "sub        $1, %%ecx;"
        "mov        %%ecx, %%edx;"
        "sub        %0, %%edx;"
        "mov        %%edx, %1;"
        "xor        %%ecx, %1;"
        : "=r" (b)
        : "r" (a));

    printf("%d\n", b);
}
