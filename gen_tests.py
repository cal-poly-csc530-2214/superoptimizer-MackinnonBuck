import subprocess
import sys
import os
import errno

TEST_FOLDER = 'tests'
TEST_TEMPLATE = '''#include <stdio.h>

int main()
{
    int a, b;
    scanf("%%d", &a);

    asm(
        %s
        : "=r" (b)
        : "r" (a));

    printf("%%d\\n", b);
}
'''

if len(sys.argv) != 2:
    print(f'Usage: python {sys.argv[0]} <instruction_count>')
    exit(1)

proc = subprocess.Popen(['./aha', sys.argv[1]], stdout=subprocess.PIPE)
out = proc.communicate()[0].decode('utf-8')

try:
    os.makedirs('tests')
except OSError as e:
    if e.errno != errno.EEXIST:
        exit(1)

current_test_id = 0

for program in out.split('asm(')[1:]:
    program = program.split(');')[0]
    program = program.strip()
    program = program.replace(' ' * 3, ' ' * 8)
    program = TEST_TEMPLATE % program

    with open(f'{TEST_FOLDER}/test{current_test_id}.c', 'w') as file:
        file.write(program)
    
    current_test_id += 1
