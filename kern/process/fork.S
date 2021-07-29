#include <asm/regdef.h>
#include <asm/loongisa_csr.h>
#include <memlayout.h>


.extern exception_return

.section .text 

.global forkrets
.type forkrets, @function
forkrets:
  addi.w a0, sp, -16
  b exception_return
.end forkrets
