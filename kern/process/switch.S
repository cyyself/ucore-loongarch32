#include <asm/regdef.h>

.text
.globl switch_to
switch_to:
//save the registers
    st.w    sp, a0, 48
    st.w    fp, a0, 44
    st.w    ra, a0, 40
    st.w    tp, a0, 36
    st.w	s8, a0, 32
    st.w	s7, a0, 28
    st.w	s6, a0, 24
    st.w	s5, a0, 20
    st.w	s4, a0, 16
    st.w	s3, a0, 12
    st.w	s2, a0, 8
    st.w	s1, a0, 4
    st.w	s0, a0, 0

    //use as nop
    dbar    0

//restore the registers frome another struct context
    ld.w    s0, a0, 0
    ld.w    s1, a0, 4
    ld.w    s2, a0, 8
    ld.w    s3, a0, 12
    ld.w    s4, a0, 16
    ld.w    s5, a0, 20
    ld.w    s6, a0, 24
    ld.w    s7, a0, 28
    ld.w    s8, a0, 32
    ld.w    tp, a0, 36
    ld.w    ra, a0, 40
    ld.w    fp, a0, 44
    ld.w    sp, a0, 48

    dbar    0

    addi.w  t0, ra,0
    jirl    ra, t0, 0

    .end switch_to









    