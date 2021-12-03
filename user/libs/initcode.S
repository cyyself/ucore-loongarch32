#include <asm/regdef.h>
.text
.globl _start
_start:
    nop
    addi.w sp, sp, -16
    b umain
    nop

