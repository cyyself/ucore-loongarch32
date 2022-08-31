#ifndef __KERN_DRIVER_INTR_H__
#define __KERN_DRIVER_INTR_H__

#include <asm/loongisa_csr.h>

void intr_enable(void);
void intr_disable(void);


static inline unsigned long __intr_save(void)
{
	return __csrxchg(0, LISA_CSR_CRMD_IE, LISA_CSR_CRMD);
}

static inline void __intr_restore(unsigned long flags)
{
	__csrxchg(flags, LISA_CSR_CRMD_IE, LISA_CSR_CRMD);
}

#define local_intr_save(x)      do { x = __intr_save(); } while (0)
#define local_intr_restore(x)   __intr_restore(x);

#endif /* !__KERN_DRIVER_INTR_H__ */

