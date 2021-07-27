.extern ramExcHandle_general
.section .ramexv
.global __exception_vector
__exception_vector:
    .space 16384 // ebase + 0x4000
    // exception start
    b ramExcHandle_general // vector 32
    .space 16380 // + 32*512 - 4
.global __exception_vector_end
__exception_vector_end: