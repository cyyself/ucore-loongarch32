#include <asm/regdef.h>
.text
.globl _start
#.extern _gp
# TODO: port to loongarch32
_start:
    nop
    #la $gp, _gp
    addi.w sp, sp, -16
    b umain
    nop

