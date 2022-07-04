#ifndef __LIBS_LOONGARCH_H__
#define __LIBS_LOONGARCH_H__

#include <defs.h>

#define do_div(n, base) ({                                          \
            unsigned long __mod;    \
            (__mod) = ((unsigned long)n) % (base);                                \
            (n) = ((unsigned long)n) / (base);                                          \
            __mod;                                                  \
        })

#define barrier() __asm__ __volatile__ ("" ::: "memory")

#define __read_reg(source) (\
    {int __res;\
    __asm__ __volatile__("move %0, " #source "\n\t"\
      : "=r"(__res));\
    __res;\
    })

static inline unsigned int __mulu10(unsigned int n)
{
    return (n<<3)+(n<<1);
}

/* __divu* routines are from the book, Hacker's Delight */

static inline unsigned int __divu10(unsigned int n) {
    unsigned int q, r;
    q = (n >> 1) + (n >> 2);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);
    q = q >> 3;
    r = n - __mulu10(q);
    return q + ((r + 6) >> 4);
}

static inline unsigned __divu5(unsigned int n) {
    unsigned int q, r;
    q = (n >> 3) + (n >> 4);
    q = q + (q >> 4);
    q = q + (q >> 8);
    q = q + (q >> 16);
    r = n - q*5;
    return q + (13*r >> 6);
}


static inline uint8_t inb(uint32_t port) __attribute__((always_inline));
static inline void outb(uint32_t port, uint8_t data) __attribute__((always_inline));
static inline uint32_t inw(uint32_t port) __attribute__((always_inline));
static inline void outw(uint32_t port, uint32_t data) __attribute__((always_inline));

static inline uint8_t
inb(uint32_t port) {
    uint8_t data = *((const volatile uint8_t*) port);
    return data;
}

static inline void
outb(uint32_t port, uint8_t data) {
    *((volatile uint8_t*) port) = data;
}

static inline uint32_t
inw(uint32_t port) {
    uint32_t data = *((volatile uintptr_t *) port);
    return data;
}

static inline void
outw(uint32_t port, uint32_t data) {
    *((volatile uintptr_t *) port) = data;
}


/* board specification */
#define COM1            0x9fe001e0
#define COM1_IRQ        3
#define COM1_BAUD_DDL   0x23

#define TIMER0_IRQ      11

#define CACHELINE_SIZE  16

static void fence_i(void *va_start, int size) {
    /*
        The fence_i function is used for make d-cache sync to i-cache so we can correctly execute modified code.

        As loongarch32 didn't guarantee any cache coherence, we need to make dirty d-cache writeback to memory and invalidte it from i-cache. 

        This operation is not necessary when running on ISA level emulator like QEMU, but it must be done when running on real hardware or FPGA.
     */
    asm volatile(".word 0b00111000011100100000000000000000"); // dbar, used for out-of-order machine
    void *va_end = va_start + size;
    while (va_start < va_end) {
        asm volatile("cacop 9, %0 ,0": "=r"(va_start)); // code[2:0]=1->d-cache, code[4:3]=2->index invalidate and writeback
        asm volatile("cacop 8, %0 ,0": "=r"(va_start)); // code[2:0]=0->i-cache, code[4:3]=2->index invalidate
        va_start += CACHELINE_SIZE;
    }
    asm volatile(".word 0b00111000011100101000000000000000"); // ibar, used for flush pipeline and instruction buffer.
}

#endif /* !__LIBS_LOONGARCH_H__ */

