#include <asm/regdef.h>
#include <asm/loongisa_csr.h>
#include <memlayout.h>

//.extern current
.extern loongarch_trap

.section .text
.global ramExcHandle_general
.global exception_return

.type ramExcHandle_general, @function
ramExcHandle_general:
    // Save t0 and t1
	csrwr   t0, LISA_CSR_KS0
    csrwr   t1, LISA_CSR_KS1
    // Save previous stack pointer in t1
    move    t1, sp
    csrwr   t1, LISA_CSR_KS2
    //t1 saved the vaual of KS2,KS2 saved sp
    /*
        Warning: csrwr will bring the old csr register value into rd, 
        not only just write rd to csr register,
        so you may see the rd changed.
        It's documented in the manual from loongarch.
    */

    // check if user mode
    csrrd   t0, LISA_CSR_PRMD  
    andi    t0, t0, 3
    beq     t0, zero, 1f

    
    /* Coming from user mode - load kernel stack into sp */
    /* TODO: add proc and uncomment here
    la      t0, current
    ld.w    t0, t0, 0
    ld.w    t0, t0, 12
    lu12i.w t1, 1
    slli.w  t1, t1, 13 // KSTACKSIZE=8192
    add.w   sp, t0, t1
    csrrd   t1, LISA_CSR_KS2
    // addi.w  sp, t0, KSTACKSIZE
    */
1:
    //saved EXST to t0 for save EXST to sp later(line 114) 
    csrrd   t0, LISA_CSR_EXST
    //return KS2
    csrrd   t1, LISA_CSR_KS2
    b common_exception


common_exception:
   /*
    * At this point:
    *      Interrupts are off. (The processor did this for us.)
    *      t0 contains the exception status(like exception cause on MIPS).
    *      t1 contains the old stack pointer.
    *      sp points into the kernel stack.
    *      All other registers are untouched.
    */
   
   /*
    * Allocate stack space for 34 words to hold the trap frame,
    * plus four more words for a minimal argument block.
    */
    addi.w  sp, sp, -156
    st.w    s7, sp, 148
    st.w    s6, sp, 144
    st.w    s5, sp, 140
    st.w    s4, sp, 136
    st.w    s3, sp, 132
    st.w    s2, sp, 128
    st.w    s1, sp, 124
    st.w    s0, sp, 120
    st.w    fp, sp, 116
    st.w    t8, sp, 112
    st.w    t7, sp, 108
    st.w    t6, sp, 104
    st.w    t5, sp, 100
    st.w    t4, sp, 96
    st.w    t3, sp, 92
    st.w    t2, sp, 88
    //st.w    t1, sp, 84
    //st.w    t0, sp, 80
    st.w    a7, sp, 76
    st.w    a6, sp, 72
    st.w    a5, sp, 68
    st.w    a4, sp, 64
    st.w    a3, sp, 60
    st.w    a2, sp, 56
    st.w    a1, sp, 52
    st.w    a0, sp, 48
    st.w    v1, sp, 44
    st.w    v0, sp, 40
    st.w    t1, sp, 36 // replace sp with real sp, now use t1 for free
    st.w    tp, sp, 32
    // save real t0 and t1 after real sp (stored in t1 previously) stored
    csrrd   t1, LISA_CSR_KS1
    st.w    t1, sp, 84
    csrrd   t1, LISA_CSR_KS0
    st.w    t1, sp, 80
    
    // replace with real value
    // save tf_era after t0 and t1 saved
    csrrd   t1, LISA_CSR_EPC
    st.w    t1, sp, 152

   /*
    * Save remaining exception context information.
    */

    // save ra
    st.w    ra, sp, 28
    // save prmd
    csrrd   t1, LISA_CSR_PRMD
    st.w    t1, sp, 24
    // save estat
    st.w    t0, sp, 20
    // now use t0 for free
    // store badv
    csrrd   t0, LISA_CSR_BADV
    st.w    t0, sp, 16
    st.w    zero, sp, 12
    // support nested interrupt

    // IE and PLV will automatically set to 0 when trap occur

    // set trapframe as function argument
    addi.w  a0, sp, 16
	li	t0, 0xb0	# PLV=0, IE=0, PG
	csrwr	t0, LISA_CSR_CRMD
    la.abs  t0, loongarch_trap
    jirl    ra, t0, 0
    //bl loongarch_trap



exception_return:
    // restore prmd
    ld.w    t0, sp, 24
    li      t1, 7
    csrxchg t0, t1, LISA_CSR_PRMD
    // restore era no k0 and k1 for la32, so must do first
    ld.w    t0, sp, 152
    csrwr   t0, LISA_CSR_EPC
    // restore general registers
    ld.w    ra, sp, 28
    ld.w    tp, sp, 32
    //ld.w    sp, sp, 36 (do it finally)
    ld.w    v0, sp, 40
    ld.w    v1, sp, 44
    ld.w    a0, sp, 48
    ld.w    a1, sp, 52
    ld.w    a2, sp, 56
    ld.w    a3, sp, 60
    ld.w    a4, sp, 64
    ld.w    a5, sp, 68
    ld.w    a6, sp, 72
    ld.w    a7, sp, 76
    ld.w    t0, sp, 80
    ld.w    t1, sp, 84
    ld.w    t2, sp, 88
    ld.w    t3, sp, 92
    ld.w    t4, sp, 96
    ld.w    t5, sp, 100
    ld.w    t6, sp, 104
    ld.w    t7, sp, 108
    ld.w    t8, sp, 112
    ld.w    fp, sp, 116
    ld.w    s0, sp, 120
    ld.w    s1, sp, 124
    ld.w    s2, sp, 128
    ld.w    s3, sp, 132
    ld.w    s4, sp, 136
    ld.w    s5, sp, 140
    ld.w    s6, sp, 144
    ld.w    s7, sp, 148
    // restore sp
    ld.w    sp, sp, 36
    ertn

    .end exception_return
    .end common_exception




