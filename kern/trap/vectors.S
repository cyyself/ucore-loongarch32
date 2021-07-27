.extern ramExcHandle_general
.section .ramexv
.global __exception_vector
__exception_vector:
    .space 16384
    // exception start
    b ramExcHandle_general
    .space 23548
.global __exception_vector_end
__exception_vector_end: