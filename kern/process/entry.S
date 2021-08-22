#include <asm/regdef.h>

.text
.globl kernel_thread_entry

/* $a0 = arg, $a1 = func
 * see proc.c:kernel_thread
*/
kernel_thread_entry:
    addi.w  sp, sp,  -16
    //goto kernel_thread
    jirl    ra, a1, 0
   // bl a1
    move    a0, v0
    //goto do_exit():see proc.c
    la.abs  t0, do_exit 
    jirl    ra, t0, 0


