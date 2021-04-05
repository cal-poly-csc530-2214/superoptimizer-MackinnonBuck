#include <stdio.h>

int main()
{
    int a, b;
    scanf("%d", &a);

    asm(
        "mov        %0, %%ebx;"
        "shr        $31, %%ebx;"
        "mov        %%ebx, %%ecx;"
        "neg        %%ecx;"
        "mov        %0, %%edx;"
        "sub        %%ebx, %%edx;"
        "mov        %%edx, %1;"
        "xor        %%ecx, %1;"
        : "=r" (b)
        : "r" (a));

    printf("%d\n", b);
}
