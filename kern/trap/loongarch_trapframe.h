#include <defs.h>
#ifndef _LA32_TRAPFRAME_H_
#define _LA32_TRAPFRAME_H_
/* $2 - $31 */
struct pushregs {
    uint32_t reg_r[30];
};

struct trapframe {
    uint32_t tf_badv; // CSR[0x7]
    uint32_t tf_estat; // CSR[0x5]
    uint32_t tf_prmd; // CSR[0x1]
    uint32_t tf_ra;
    struct pushregs tf_regs;
    uint32_t tf_era; // CSR[0x6] same as epc on mips
};

/*
 * LA32 exception codes.
 */
#define EX_IRQ    0    /* Interrupt */

#endif /* _LA32_TRAPFRAME_H_ */