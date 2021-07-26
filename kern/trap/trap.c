#include <trap.h>
#include <stdio.h>
#include <string.h>
#include <loongarch_trapframe.h>

#define EXCCODE_GENERIC 32
#define vec_size 512

static void local_flush_icache_range(unsigned long start, unsigned long end)
{
	asm volatile ("\tibar 0\n"::);
}

/* Install CPU exception handler */
void set_handler(unsigned long offset, void *addr, unsigned long size)
{
    extern unsigned char __exception_vector[];
	memcpy((void *)(__exception_vector + offset), addr, size);
	local_flush_icache_range(__exception_vector + offset, __exception_vector + offset + size);
}

void set_exception_handler() {
    extern unsigned char __exception_vector[], ramExcHandle_general[];
    kprintf("ebase = 0x%x\n",__exception_vector);
    set_handler(EXCCODE_GENERIC * vec_size , &ramExcHandle_general, vec_size);
}

void
loongarch_trap(struct trapframe *tf) {
    kprintf("epc = %x",tf->tf_era);
}