#include <asm/regdef.h>
#include <asm/loongisa_csr.h>

.global kernel_entry
.extern kern_init

.extern edata
.extern end

.section .startup

#define KSTACKSIZE (4096*2)

kernel_entry:
  b reset
  nop
	.align 4
reset:
    la t0, 1f
    jirl zero, t0, 0
1:
    la sp, bootstacktop
# TODO: Exception Vector
#zero bss
    la t0, edata
    la t1, end
2:
    st.w zero, t0, 0
    addi.w t0, t0, 4
    slt t3, t0, t1
    bgtz t3, 2b
# TODO: CSR Write
    addi.w sp, sp, -16
	# Config direct window and set PG
	li	t0, 0x80000011
	csrwr	t0, LISA_CSR_DMWIN0
	// DMWIN0: 0x80000000-0x9fffffff->Direct Cached PLV0
	li      t0, 0xaa000001
	csrwr	t0, LISA_CSR_DMWIN1
    // DMWIN1: 0xa0000000-0xbfffffff->Direct Uncached PLV0(Serial Port is here)
	# Enable PG
	li	t0, 0xb0	# PLV=0, IE=0, PG
	csrwr	t0, LISA_CSR_CRMD
    b kern_init

.section .data
    .global bootstack
bootstack:
    .space KSTACKSIZE
    .global bootstacktop
bootstacktop:
    .space 32