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
        "xor        %0, %%edx;"
        "mov        %%ecx, %1;"
        "sub        %%edx, %1;"
        : "=r" (b)
        : "r" (a));

    printf("%d\n", b);
}
