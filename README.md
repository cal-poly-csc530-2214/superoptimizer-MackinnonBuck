## Aha! x86 experimentation.

### Summary
I decided to start by playing with Henry Warren's original "Aha!" implementation. I wanted to run some of the generated assembly on my machine, but this wasn't initially possible since the general-purpose RISC ISA that the original implementation uses is not the ISA that my computer's CPU runs (x86). The paper mentioned that it should be easy to port, so I though I would try doing an x86 port of "Aha!".

### Changes
I started by modifying `aha.h`, adding/removing/reimplementing certain instructions to match the x86 ISA. This was a relatively small task since it amounted to mostly renaming instructions and adding new ones similar to those that already exist (like a right rotate). However, I quickly realized that a fundamental difference between x86 and the original ISA is that x86 uses the first operand as both a source and a destination. The original implementation seemed to make the assumption that the ISA being used would support each new instruction writing to a currently unused destination register, rather than the value of the first operand being used and overwritten with the result. Not to worry, there is a simple (albiet unoptimal) workaround: Move the first source operand into a new register, then use that new register as the first operand in the next instruction. For example:

This:
```
shr   r1,rx,31
sub   r2,rx,r1
sub   r3,r2,rx
xor   r4,r3,r2
```

Becomes this:
```
mov   r1, rx
shr   r1, 31
mov   r2, rx
sub   r2, r1
mov   r3, r2
sub   r3, rx
mov   r4, r3
xor   r4, r2
```

This change didn't require modifying the core functionality of "Aha!", but could be done instead by reformatting how each solution is outputted.

I considered trying to implement an optimization technique that uses fewer registers and eliminates some of these moves, but given the time constraint, I decided to instead focus on getting some of the superoptimized code to run (since that was my original goal).

After manually re-formatting some solutions for GCC's inline assembly blocks (which was not as trivial as I would've thought, due to weird conventions like the destination registers coming last), I was able to run some superoptimized assembly on my machine. However, manually reformatting was tedious and I wanted to test more than just a few examples, so I modified Aha!'s output to be formatted in a way that could be copied and pasted into a GCC inline assembly block:

```
Found a 4-operation program:
asm(
   "mov   %0, %%ebx;"
   "shr   $31, %%ebx;"
   "mov   %0, %%ecx;"
   "sub   %%ebx, %%ecx;"
   "mov   %%ecx, %%edx;"
   "sub   %0, %%edx;"
   "mov   %%edx, %1;"
   "xor   %%ecx, %1;"
);
   Expr: (((x - (x >>u 31)) - x) ^ (x - (x >>u 31)))
```

To top things off, I wrote a Python script that could parse Aha!'s output and generate a `.c` file for each solution. For example:

```
#include <stdio.h>

int main()
{
    int a, b;
    scanf("%d", &a);

    asm(
        "mov        %0, %%ebx;"
        "shr        $31, %%ebx;"
        "mov        %0, %%ecx;"
        "sub        %%ebx, %%ecx;"
        "mov        %%ecx, %%edx;"
        "sub        %0, %%edx;"
        "mov        %%edx, %1;"
        "xor        %%ecx, %1;"
        : "=r" (b)
        : "r" (a));

    printf("%d\n", b);
}
```

And this ended up being the extent of the progress I made. I was hoping to start trying to get rid of some `mov` instructions, but it seemed like it wasn't something that could be finished in reasonable further time.