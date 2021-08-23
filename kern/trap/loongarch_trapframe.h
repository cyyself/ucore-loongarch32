#include <defs.h>
#ifndef _LA32_TRAPFRAME_H_
#define _LA32_TRAPFRAME_H_
/* $2 - $31 */
struct pushregs {
    uint32_t reg_r[30];
};


#define LOONGARCH_REG_START  (0)
#define LOONGARCH_REG_TP    (LOONGARCH_REG_START+0)
#define LOONGARCH_REG_SP    (LOONGARCH_REG_START+1)
#define LOONGARCH_REG_V0    (LOONGARCH_REG_START+2)
#define LOONGARCH_REG_V1    (LOONGARCH_REG_START+3)
#define LOONGARCH_REG_A0    (LOONGARCH_REG_START+2)
#define LOONGARCH_REG_A1    (LOONGARCH_REG_START+3)
#define LOONGARCH_REG_A2    (LOONGARCH_REG_START+4)
#define LOONGARCH_REG_A3    (LOONGARCH_REG_START+5)
#define LOONGARCH_REG_A4    (LOONGARCH_REG_START+6)
#define LOONGARCH_REG_A5    (LOONGARCH_REG_START+7)
#define LOONGARCH_REG_A6    (LOONGARCH_REG_START+8)
#define LOONGARCH_REG_A7    (LOONGARCH_REG_START+9)
#define LOONGARCH_REG_T0    (LOONGARCH_REG_START+10)
#define LOONGARCH_REG_T1    (LOONGARCH_REG_START+11)
#define LOONGARCH_REG_T2    (LOONGARCH_REG_START+12)
#define LOONGARCH_REG_T3    (LOONGARCH_REG_START+13)
#define LOONGARCH_REG_T4    (LOONGARCH_REG_START+14)
#define LOONGARCH_REG_T5    (LOONGARCH_REG_START+15)
#define LOONGARCH_REG_T6    (LOONGARCH_REG_START+16)
#define LOONGARCH_REG_T7    (LOONGARCH_REG_START+17)
#define LOONGARCH_REG_T8    (LOONGARCH_REG_START+18)

#define LOONGARCH_REG_FP    (LOONGARCH_REG_START+20)
#define LOONGARCH_REG_S0    (LOONGARCH_REG_START+21)
#define LOONGARCH_REG_S1    (LOONGARCH_REG_START+22)
#define LOONGARCH_REG_S2    (LOONGARCH_REG_START+23)
#define LOONGARCH_REG_S3    (LOONGARCH_REG_START+24)
#define LOONGARCH_REG_S4    (LOONGARCH_REG_START+25)
#define LOONGARCH_REG_S5    (LOONGARCH_REG_START+26)
#define LOONGARCH_REG_S6    (LOONGARCH_REG_START+27)
#define LOONGARCH_REG_S7    (LOONGARCH_REG_START+28)
#define LOONGARCH_REG_S8    (LOONGARCH_REG_START+29)



struct trapframe {
    uint32_t tf_badv; // CSR[0x7]
    uint32_t tf_estat; // CSR[0x5]
    uint32_t tf_prmd; // CSR[0x1]
    uint32_t tf_ra;
    struct pushregs tf_regs;
    uint32_t tf_era; // CSR[0x6] same as epc on LOONGARCH
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
#define EX_ADE			8	
#define EX_UNALIGN		9	
#define EX_OOB			10  
#define EX_SYS			11	/* System call */
#define EX_BP			12	/* Breakpoint */
#define EX_RI			13	/* Reserved inst */
#define EX_IPE			14	/* Inst. Privileged Error */
#define EX_FPDIS		15	
#define EX_LSXDIS		16  
#define EX_LASXDIS		17	
#define EX_FPE			18	/* Floating Point Exception */
#define EXCSUBCODE_FPE			0	/* Floating Point Exception */
#define EXCSUBCODE_VEC			1	/* Vector Exception */
#define EX_WATCH		19
#define EX_BTDIS		20
#define EX_BTE			21	/* Binary Trans. Exception */
#define EX_PSI			22
#define EX_HYP			23
#define EX_FC			24
#define EXCSUBCODE_SFC			0
#define EXCSUBCODE_HFC			1
#define EX_SE			25
#define EX_TLBR         0x3f

#endif /* _LA32_TRAPFRAME_H_ */