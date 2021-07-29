#include <asm/regdef.h>

.text
.globl kernel_thread_entry

/* $a0 = arg, $a1 = func
 * see proc.c:kernel_thread
*/
kernel_thread_entry:
    addi.w  sp, sp,  -16
    //goto kernel_thread
    addi.w  t0, a1, 0
  //  la.abs  t0, a1
    jirl    ra, t0, 0
   // bl a1
    move    v0, a0
    //goto do_exit():see proc.c
    la.abs  t0, do_exit 
    jirl    ra, t0, 0


