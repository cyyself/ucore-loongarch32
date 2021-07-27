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
#define EX_IRQ			0	/* Interupt */
#define EX_TLBL		1	/* TLB miss on a load */
#define EX_TLBS		2	/* TLB miss on a store */
#define EX_TLBI		3	/* TLB miss on a ifetch */
#define EX_MOD			4	/* TLB modified fault */
#define EX_TLBRI		5	/* TLB Read-Inhibit exception */
#define EX_TLBXI		6	/* TLB Execution-Inhibit exception */
#define EX_TLBPE		7	/* TLB Privilege Error */
#define EX_ADE			8	/* TODO: ???? */
#define EX_UNALIGN		9	/* TODO: ???? */
#define EX_OOB			10  /* TODO: ???? out of bounds */
#define EX_SYS			11	/* System call */
#define EX_BP			12	/* Breakpoint */
#define EX_RI			13	/* Reserved inst */
#define EX_IPE			14	/* Inst. Privileged Error */
#define EX_FPDIS		15	/* TODO: ???? */
#define EX_LSXDIS		16  /* TODO: ???? */
#define EX_LASXDIS		17	/* TODO: ???? */
#define EX_FPE			18	/* Floating Point Exception */
#define EXCSUBCODE_FPE			0	/* Floating Point Exception */
#define EXCSUBCODE_VEC			1	/* Vector Exception */
#define EX_WATCH		19
#define EX_BTDIS		20	/* TODO: ???? */
#define EX_BTE			21	/* Binary Trans. Exception */
#define EX_PSI			22	/* TODO: ???? */
#define EX_HYP			23	/* TODO: ???? */
#define EX_FC			24	/* TODO: ???? */
#define EXCSUBCODE_SFC			0	/* TODO: ???? */
#define EXCSUBCODE_HFC			1	/* TODO: ???? */
#define EX_SE			25	/* TODO: ???? */
#define EX_TLBR         0x3f

#endif /* _LA32_TRAPFRAME_H_ */