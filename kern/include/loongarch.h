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

#define __swizzle_addr_b(port)	(port)
#define __swizzle_addr_w(port)	(port)
#define __swizzle_addr_l(port)	(port)
#define __swizzle_addr_q(port)	(port)

#ifndef __io_br
#define __io_br()      barrier()
#endif

#define war_io_reorder_wmb()		barrier()
#define ioswabb(a, x)		(x)
#ifndef HAVE_ARCH_BUG
#define BUG() do {} while (1)
#endif

#ifndef HAVE_ARCH_BUG_ON
#define BUG_ON(condition) do { if (condition) BUG(); } while (0)
#endif

#define __BUILD_MEMORY_SINGLE(pfx, bwlq, type, irq)			\
									\
static inline void pfx##write##bwlq(type val,				\
				    volatile void *mem)		\
{									\
	volatile type *__mem;						\
	type __val;							\
									\
	war_io_reorder_wmb();					\
									\
	__mem = (void *)__swizzle_addr_##bwlq((unsigned long)(mem));	\
									\
	__val = pfx##ioswab##bwlq(__mem, val);				\
									\
	if (sizeof(type) != sizeof(unsigned long long) || sizeof(unsigned long long) == sizeof(long)) \
		*__mem = __val;						\
	else								\
		BUG();							\
}									\
									\
static inline type pfx##read##bwlq(const volatile void *mem)	\
{									\
	volatile type *__mem;						\
	type __val;							\
									\
	__mem = (void *)__swizzle_addr_##bwlq((unsigned long)(mem));	\
									\
	if (sizeof(type) != sizeof(unsigned long long) || sizeof(unsigned long long) == sizeof(long)) \
		__val = *__mem;						\
	else {								\
		__val = 0;						\
		BUG();							\
	}								\
									\
	/* prevent prefetching of coherent DMA data prematurely */	\
	return pfx##ioswab##bwlq(__mem, __val);				\
}

#define __BUILD_IOPORT_SINGLE(pfx, bwlq, type, p, slow)			\
									\
static inline void pfx##out##bwlq##p(type val, unsigned long port)	\
{									\
	volatile type *__addr;						\
	type __val;							\
									\
	war_io_reorder_wmb();					\
									\
	__addr = (void *)__swizzle_addr_##bwlq(mips_io_port_base + port); \
									\
	__val = pfx##ioswab##bwlq(__addr, val);				\
									\
	/* Really, we want this to be atomic */				\
	BUILD_BUG_ON(sizeof(type) > sizeof(unsigned long));		\
									\
	*__addr = __val;						\
	slow;								\
}									\
									\
static inline type pfx##in##bwlq##p(unsigned long port)			\
{									\
	volatile type *__addr;						\
	type __val;							\
									\
	__addr = (void *)__swizzle_addr_##bwlq(mips_io_port_base + port); \
									\
	BUILD_BUG_ON(sizeof(type) > sizeof(unsigned long));		\
									\
	__val = *__addr;						\
	slow;								\
									\
	/* prevent prefetching of coherent DMA data prematurely */	\
	rmb();								\
	return pfx##ioswab##bwlq(__addr, __val);			\
}


#define __BUILD_MEMORY_PFX(bus, bwlq, type)				\
									\
__BUILD_MEMORY_SINGLE(bus, bwlq, type, 1)

#define BUILDIO_MEM(bwlq, type)						\
									\
__BUILD_MEMORY_PFX(__raw_, bwlq, type)					\
__BUILD_MEMORY_PFX(, bwlq, type)					\
__BUILD_MEMORY_PFX(__mem_, bwlq, type)					\

BUILDIO_MEM(b, uint8_t)
BUILDIO_MEM(l, uint32_t)

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

//TODO: #define TIMER0_IRQ       7


#endif /* !__LIBS_LOONGARCH_H__ */

