#ifndef __ASM_MACH_LOONGSON32_LOONGISA_CSR_H_
#define __ASM_MACH_LOONGSON32_LOONGISA_CSR_H_

#ifdef __ASSEMBLY__
#define _ULCAST_
#define _U64CAST_
#else
#define _ULCAST_ (unsigned long)
#define _U64CAST_ (u64)
#endif

/* major opcode for CSR instruction */
#define LISA_OPC_MAJOR_CSR	0

/* minor opcode for CSR instruction */
#define LISA_OPC_MINOR_CSRL	0x10
#define LISA_OPC_MINOR_CSRH	0x11
#define LISA_OPC_MINOR_CSR	0x12
#define LISA_OPC_MINOR_GCSRL	0x13
#define LISA_OPC_MINOR_GCSR	0x15

/* CSR register number */

/* basic CSR register */
#define LISA_CSR_CRMD		0x0	// 32   current mode info
#define CSR_CRMD_DACM_SHIFT	7
#define CSR_CRMD_DACM_WIDTH	2
#define CSR_CRMD_DACM		(_ULCAST_(0x3) << CSR_CRMD_DACM_SHIFT)
#define CSR_CRMD_DACF_SHIFT	5
#define CSR_CRMD_DACF_WIDTH	2
#define CSR_CRMD_DACF		(_ULCAST_(0x3) << CSR_CRMD_DACF_SHIFT)
#define CSR_CRMD_PG_SHIFT	4
#define CSR_CRMD_PG		(_ULCAST_(0x1) << CSR_CRMD_PG_SHIFT)
#define CSR_CRMD_DA_SHIFT	3
#define CSR_CRMD_DA		(_ULCAST_(0x1) << CSR_CRMD_DA_SHIFT)
#define CSR_CRMD_IE_SHIFT	2
#define CSR_CRMD_IE		(_ULCAST_(0x1) << CSR_CRMD_IE_SHIFT)
#define CSR_CRMD_PLV_SHIFT	0
#define CSR_CRMD_PLV_WIDTH	2
#define CSR_CRMD_PLV		(_ULCAST_(0x3) << CSR_CRMD_PLV_SHIFT)
#define PLV_USER		3	// TODO: temp
#define PLV_KERN		0
#define PLV_MASK		0x3

#define LISA_CSR_PRMD		0x1	// 32   prev-exception mode info
#define CSR_PRMD_PIE_SHIFT	2
#define CSR_PRMD_PIE		(_ULCAST_(0x1) << CSR_PRMD_PIE_SHIFT)
#define CSR_PRMD_PPLV_SHIFT	0
#define CSR_PRMD_PPLV_WIDTH	2
#define CSR_PRMD_PPLV		(_ULCAST_(0x3) << CSR_PRMD_PPLV_SHIFT)

#define LISA_CSR_CU		0x2	// 32   coprocessor enable
#define CSR_CU_LBTEN_SHIFT	3
#define CSR_CU_LBTEN		(_ULCAST_(0x1) << CSR_CU_LBTEN_SHIFT)
#define CSR_CU_LASXEN_SHIFT	2
#define CSR_CU_LASXEN		(_ULCAST_(0x1) << CSR_CU_LASXEN_SHIFT)
#define CSR_CU_LSXEN_SHIFT	1
#define CSR_CU_LSXEN		(_ULCAST_(0x1) << CSR_CU_LSXEN_SHIFT)
#define CSR_CU_FPEN_SHIFT	0
#define CSR_CU_FPEN		(_ULCAST_(0x1) << CSR_CU_FPEN_SHIFT)

#define LISA_CSR_CFG		0x3	// 32   config

#define LISA_CSR_EXCFG		0x4	// 32   exception config
#define CSR_EXCFG_VS_SHIFT	16
#define CSR_EXCFG_VS_WIDTH	3
#define CSR_EXCFG_VS		(_ULCAST_(0x7) << CSR_EXCFG_VS_SHIFT)
#define CSR_EXCFG_IM_SHIFT	0
#define CSR_EXCFG_IM_WIDTH	13
#define CSR_EXCFG_IM		(_ULCAST_(0x1fff) << CSR_EXCFG_IM_SHIFT)

#define LISA_CSR_EXST		0x5	// 32   exception status
#define CSR_EXST_ESUBCODE_SHIFT	22
#define CSR_EXST_ESUBCODE_WIDTH	9
#define CSR_EXST_ESUBCODE	(_ULCAST_(0x1ff) << CSR_EXST_ESUBCODE_SHIFT)
#define CSR_EXST_EXC_SH		16
#define CSR_EXST_EXC_WIDTH	5
#define CSR_EXST_EXC		(_ULCAST_(0x1f) << CSR_EXST_EXC_SH)
#define CSR_EXST_IS_SHIFT	0
#define CSR_EXST_IS_WIDTH	15
#define CSR_EXST_IS		(_ULCAST_(0x7fff) << CSR_EXST_IS_SHIFT)

#define LISA_CSR_EPC		0x6	// 64   error PC

#define LISA_CSR_BADV		0x7	// 64   bad virtual address

#define LISA_CSR_BADI		0x8	// 32   bad instruction

#define LISA_CSR_EBASE		0xc	// 64   exception enter base address

#define CU_FPE			1
#define CU_LSXE			(1 << 1)
#define CU_LASXE		(1 << 2)
#define CU_LBTE			(1 << 3)


/* TLB related CSR register : start with TLB if no pagewalk */
#define LISA_CSR_TLBIDX		0x10	// 32   TLB Index, EHINV, PageSize, is_gtlb
#define CSR_TLBIDX_PS_SHIFT	24
#define CSR_TLBIDX_PS_WIDTH	6
#define CSR_TLBIDX_PS		(_ULCAST_(0x3f) << CSR_TLBIDX_PS_SHIFT)
#define CSR_TLBIDX_IDX_SHIFT	0
#define CSR_TLBIDX_IDX_WIDTH	12
#define CSR_TLBIDX_IDX		(_ULCAST_(0xfff) << CSR_TLBIDX_IDX_SHIFT)
#define CSR_TLBIDX_SIZEM	0x3f000000
#define CSR_TLBIDX_SIZE		CSR_TLBIDX_PS_SHIFT
#define CSR_TLBIDX_IDXM		0xfff

#define LISA_CSR_TLBEHI		0x11	// 64   TLB EntryHi without ASID

#define LISA_CSR_TLBELO0	0x12	// 64   TLB EntryLo0
#define CSR_TLBLO0_RPLV_SHIFT	63
#define CSR_TLBLO0_RPLV		(_ULCAST_(0x1) << CSR_TLBLO0_RPLV_SHIFT)
#define CSR_TLBLO0_XI_SHIFT	62
#define CSR_TLBLO0_XI		(_ULCAST_(0x1) << CSR_TLBLO0_XI_SHIFT)
#define CSR_TLBLO0_RI_SHIFT	61
#define CSR_TLBLO0_RI		(_ULCAST_(0x1) << CSR_TLBLO0_RI_SHIFT)
#define CSR_TLBLO0_PFN_SHIFT	12
#define CSR_TLBLO0_PFN_WIDTH	36	// ignore lower 12bits
#define CSR_TLBLO0_PFN		(_ULCAST_(0xfffffffff) << CSR_TLBLO0_PFN_SHIFT)
#define CSR_TLBLO0_GLOBAL_SHIFT	6
#define CSR_TLBLO0_GLOBAL	(_ULCAST_(0x1) << CSR_TLBLO0_GLOBAL_SHIFT)
#define CSR_TLBLO0_CCA_SHIFT	4
#define CSR_TLBLO0_CCA_WIDTH	2
#define CSR_TLBLO0_CCA		(_ULCAST_(0x3) << CSR_TLBLO0_CCA_SHIFT)
#define CSR_TLBLO0_PLV_SHIFT	2
#define CSR_TLBLO0_PLV_WIDTH	2
#define CSR_TLBLO0_PLV		(_ULCAST_(0x3) << CSR_TLBLO0_PLV_SHIFT)
#define CSR_TLBLO0_WE_SHIFT	1
#define CSR_TLBLO0_WE		(_ULCAST_(0x1) << CSR_TLBLO0_WE_SHIFT)
#define CSR_TLBLO0_V_SHIFT	0
#define CSR_TLBLO0_V		(_ULCAST_(0x1) << CSR_TLBLO0_V_SHIFT)

#define LISA_CSR_TLBELO1	0x13	// 64 TLB EntryLo1
#define CSR_TLBLO1_RPLV_SHIFT	63
#define CSR_TLBLO1_RPLV		(_ULCAST_(0x1) << CSR_TLBLO1_RPLV_SHIFT)
#define CSR_TLBLO1_XI_SHIFT	62
#define CSR_TLBLO1_XI		(_ULCAST_(0x1) << CSR_TLBLO1_XI_SHIFT)
#define CSR_TLBLO1_RI_SHIFT	61
#define CSR_TLBLO1_RI		(_ULCAST_(0x1) << CSR_TLBLO1_RI_SHIFT)
#define CSR_TLBLO1_PFN_SHIFT	12
#define CSR_TLBLO1_PFN_WIDTH	36	// ignore lower 12bits
#define CSR_TLBLO1_PFN		(_ULCAST_(0xfffffffff) << CSR_TLBLO1_PFN_SHIFT)
#define CSR_TLBLO1_GLOBAL_SHIFT	6
#define CSR_TLBLO1_GLOBAL	(_ULCAST_(0x1) << CSR_TLBLO1_GLOBAL_SHIFT)
#define CSR_TLBLO1_CCA_SHIFT	4
#define CSR_TLBLO1_CCA_WIDTH	2
#define CSR_TLBLO1_CCA		(_ULCAST_(0x3) << CSR_TLBLO1_CCA_SHIFT)
#define CSR_TLBLO1_PLV_SHIFT	2
#define CSR_TLBLO1_PLV_WIDTH	2
#define CSR_TLBLO1_PLV		(_ULCAST_(0x3) << CSR_TLBLO1_PLV_SHIFT)
#define CSR_TLBLO1_WE_SHIFT	1
#define CSR_TLBLO1_WE		(_ULCAST_(0x1) << CSR_TLBLO1_WE_SHIFT)
#define CSR_TLBLO1_V_SHIFT	0
#define CSR_TLBLO1_V		(_ULCAST_(0x1) << CSR_TLBLO1_V_SHIFT)

#define LISA_CSR_TLBWIRED	0x14	// TLB wired
#define CSR_TLBWIRED_LOCK_WIDTH	6
#define CSR_TLBWIRED_LOCK	(_ULCAST_(0x3f) << CSR_TLBWIRED_LOCK_WIDTH)

#define LISA_ENTRYLO_RI		(_ULCAST_(1) << 61)
#define LISA_ENTRYLO_XI	(_ULCAST_(1) << 62)

#define LISA_CSR_TLBGST		0x15	// guest-related TLB
#define CSR_TLBGST_RID_SHIFT	16
#define CSR_TLBGST_RID_WIDTH	8
#define CSR_TLBGST_RID		(_ULCAST_(0xff) << CSR_TLBGST_RID_SHIFT)
#define CSR_TLBGST_TOTI_SHIFT	13
#define CSR_TLBGST_TOTI		(_ULCAST_(0x1) << CSR_TLBGST_TOTI_SHIFT)
#define CSR_TLBGST_USERID_SHIFT	12
#define CSR_TLBGST_USERID	(_ULCAST_(0x1) << CSR_TLBGST_USERID_SHIFT)
#define CSR_TLBGST_GVTLBSZ_SHIFT	0
#define CSR_TLBGST_GVTLBSZ_WIDTH	6
#define CSR_TLBGST_GVTLBSZ	(_ULCAST_(0x3f) << CSR_TLBGST_GVTLBSZ_SHIFT)

#define LISA_CSR_TLBGST2	0x16	// guest-related TLB
#define CSR_TLBGST2_RID_SHIFT	16
#define CSR_TLBGST2_RID_WIDTH	8
#define CSR_TLBGST2_RID		(_ULCAST_(0xff) << CSR_TLBGST2_RID_SHIFT)
#define CSR_TLBGST2_GTLB_SHIFT	0
#define CSR_TLBGST2_GTLB	(1 << CSR_TLBGST2_GTLB_SHIFT)

#define LISA_CSR_ASID		0x18	// 64 ASID
#define CSR_ASID_BIT_SHIFT	16	//ASIDBits
#define CSR_ASID_BIT_WIDTH	8
#define CSR_ASID_BIT		(_ULCAST_(0xff) << CSR_ASID_BIT_SHIFT)
#define CSR_ASID_ASID_SHIFT	0
#define CSR_ASID_ASID_WIDTH	10
#define CSR_ASID_ASID		(_ULCAST_(0x3ff) << CSR_ASID_ASID_SHIFT)

#define LISA_CSR_PGDL		0x19	// 64 page table base address when badv[47] = 0

#define LISA_CSR_PGDH		0x1a	// 64 page table base address when badv[47] = 1

#define LISA_CSR_PGD		0x1b	// 64 page table base

#define LISA_CSR_PWCTL0		0x1c	// 64 PWCtl0
#define CSR_PWCTL0_PTEW_SHIFT	30
#define CSR_PWCTL0_PTEW_WIDTH	2
#define CSR_PWCTL0_PTEW		(_ULCAST_(0x3) << CSR_PWCTL0_PTEW_SHIFT)
#define CSR_PWCTL0_DIR1WIDTH_SHIFT	25
#define CSR_PWCTL0_DIR1WIDTH_WIDTH	5
#define CSR_PWCTL0_DIR1WIDTH	(_ULCAST_(0x1f) << CSR_PWCTL0_DIR1WIDTH_SHIFT)
#define CSR_PWCTL0_DIR1BASE_SHIFT	20
#define CSR_PWCTL0_DIR1BASE_WIDTH	5
#define CSR_PWCTL0_DIR1BASE	(_ULCAST_(0x1f) << CSR_PWCTL0_DIR1BASE_SHIFT)
#define CSR_PWCTL0_DIR0WIDTH_SHIFT	15
#define CSR_PWCTL0_DIR0WIDTH_WIDTH	5
#define CSR_PWCTL0_DIR0WIDTH	(_ULCAST_(0x1f) << CSR_PWCTL0_DIR0WIDTH_SHIFT)
#define CSR_PWCTL0_DIR0BASE_SHIFT	10
#define CSR_PWCTL0_DIR0BASE_WIDTH	5
#define CSR_PWCTL0_DIR0BASE	(_ULCAST_(0x1f) << CSR_PWCTL0_DIR0BASE_SHIFT)
#define CSR_PWCTL0_PTWIDTH_SHIFT	5
#define CSR_PWCTL0_PTWIDTH_WIDTH	5
#define CSR_PWCTL0_PTWIDTH	(_ULCAST_(0x1f) << CSR_PWCTL0_PTWIDTH_SHIFT)
#define CSR_PWCTL0_PTBASE_SHIFT	0
#define CSR_PWCTL0_PTBASE_WIDTH	5
#define CSR_PWCTL0_PTBASE	(_ULCAST_(0x1f) << CSR_PWCTL0_PTBASE_SHIFT)

#define LISA_CSR_PWCTL1		0x1d	// 64 PWCtl1
#define CSR_PWCTL1_DIR3WIDTH_SHIFT	18
#define CSR_PWCTL1_DIR3WIDTH_WIDTH	5
#define CSR_PWCTL1_DIR3WIDTH	(_ULCAST_(0x1f) << CSR_PWCTL1_DIR3WIDTH_SHIFT)
#define CSR_PWCTL1_DIR3BASE_SHIFT	12
#define CSR_PWCTL1_DIR3BASE_WIDTH	5
#define CSR_PWCTL1_DIR3BASE	(_ULCAST_(0x1f) << CSR_PWCTL0_DIR3BASE_SHIFT)
#define CSR_PWCTL1_DIR2WIDTH_SHIFT	6
#define CSR_PWCTL1_DIR2WIDTH_WIDTH	5
#define CSR_PWCTL1_DIR2WIDTH	(_ULCAST_(0x1f) << CSR_PWCTL1_DIR2WIDTH_SHIFT)
#define CSR_PWCTL1_DIR2BASE_SHIFT	0
#define CSR_PWCTL1_DIR2BASE_WIDTH	5
#define CSR_PWCTL1_DIR2BASE	(_ULCAST_(0x1f) << CSR_PWCTL0_DIR2BASE_SHIFT)

#define LISA_CSR_FTLBPGSIZE	0x1e
#define CSR_FTLBPGSIZE_FTPS_WIDTH	6
#define CSR_FTLBPGSIZE_FTPS	(_ULCAST_(0x3f))

#define LISA_CSR_REDUCEVA	0x1f
#define CSR_REDUCEVA_RDVA_WIDTH	4
#define CSR_REDECEVA_RDVA	(_ULCAST_(0xf))

/* read only CSR register : start with CPU */
#define LISA_CSR_CPUN		0x20	// 32 CPU core number
#define CSR_CPUN_CPUNO_WIDTH	9
#define CSR_CPUN_CPUNO		_ULCAST_(0x1ff)

#define LISA_CSR_CONFIG1	0x21	// 32 CPU info
#define CSR_CONF1_VSMAX_SHIFT	12
#define CSR_CONF1_VSMAX_WIDTH	3
#define CSR_CONF1_VSMAX		(_ULCAST_(7) << CSR_CONF1_VSMAX_SHIFT)
/* stable timer bits - 1, 0x2f = 47*/
#define CSR_CONF1_STBITS_SHIFT	4
#define CSR_CONF1_STBITS_WIDTH	8
#define CSR_CONF1_STBITS	(_ULCAST_(0xff) << CSR_CONF1_STBITS_SHIFT)
#define CSR_CONF1_KSNUM_WIDTH	4
#define CSR_CONF1_KSNUM		_ULCAST_(0xf)

#define LISA_CSR_CONFIG2	0x22
#define CSR_CONF2_PGMASK_SUPP	0x3ffff000

#define LISA_CSR_CONFIG3	0x23
#define CSR_CONF3_FTLBIDX_SHIFT	20
#define CSR_CONF3_FTLBIDX_WIDTH	6
#define CSR_CONF3_FTLBIDX	(_ULCAST_(0x3f) << CSR_CONF3_FTLBIDX_SHIFT)
#define CSR_FLTB_SETS		256
#define CSR_CONF3_FTLBWAYS_SHIFT	12
#define CSR_CONF3_FTLBWAYS_WIDTH	8
#define CSR_CONF3_FTLBWAYS	(_ULCAST_(0xff) << CSR_CONF3_FTLBWAYS_SHIFT)
#define CSR_FLTBWAYS_SIZE	8
#define CSR_CONF3_VTLBSIZE_SHIFT	4
#define CSR_CONF3_VTLBSIZE_WIDTH	8
#define CSR_CONF3_VTLBSIZE	(_ULCAST_(0xff) << CSR_CONF3_VTLBSIZE_SHIFT)
/* mean VTLB 64 index */
#define CSR_VTLB_SIZE		64
#define CSR_CONF3_TLBORG_WIDTH	4
#define CSR_CONF3_TLBORG	(_ULCAST_(0xf) << CSR_CONF3_TLBORG_SHIFT)
/* mean use VTLB+FTLB */
#define TLB_ORG			2

/* Kscratch : start with KS */
#define LISA_CSR_KS0		0x30	// 64
#define LISA_CSR_KS1		0x31	// 64
#define LISA_CSR_KS2		0x32	// 64
#define LISA_CSR_KS3		0x33	// 64
#define LISA_CSR_KS4		0x34	// 64
#define LISA_CSR_KS5		0x35	// 64
#define LISA_CSR_KS6		0x36	// 64
#define LISA_CSR_KS7		0x37	// 64
#define LISA_CSR_KS8		0x38	// 64

/* timer : start with TM */
#define LISA_CSR_TMID		0x40	// 32 timer ID

#define LISA_CSR_TMCFG		0x41	// ?? timer config
#define CSR_TMCFG_VAL_SHIFT	2
#define	CSR_TMCFG_VAL_WIDTH	48
#define CSR_TMCFG_VAL		(_ULCAST_(0x3fffffffffff) << CSR_TMCFG_VAL_SHIFT)
#define CSR_TMCFG_PERIOD_SHIFT	1
#define CSR_TMCFG_PERIOD	(_ULCAST_(0x1) << CSR_TMCFG_PERIOD_SHIFT)
#define CSR_TMCFG_EN		(_ULCAST_(0x1))

#define LISA_CSR_TMTICK		0x42	// ?? timer ticks remain

#define LISA_CSR_TMOFFSET	0x43	// 64 timer offset

#define LISA_CSR_TMINTCLR	0x44	// 64 timer interrupt clear
#define CSR_TMINTCLR_TI_SHIFT	0
#define CSR_TMINTCLR_TI		(1 << CSR_TMINTCLR_TI_SHIFT)

/* guest : start with GST */
#define LISA_CSR_GSTINFO	0x50	// 32 basic guest info
#define CSR_GSTINFO_GID_SHIFT	16
#define CSR_GSTINFO_GID_WIDTH	8
#define CSR_GSTINFO_GID		(_ULCAST_(0xff) << CSR_GSTINFO_GID_SHIFT)
#define CSR_GSTINFO_GIDBIT_SHIFT	4
#define CSR_GSTINFO_GIDBIT_WIDTH	6
#define CSR_GSTINFO_GIDBIT	(_ULCAST_(0x3f) << CSR_GSTINFO_GIDBIT_SHIFT)
#define CSR_GSTINFO_PVM_SHIFT	1
#define CSR_GSTINFO_PVM		(_ULCAST_(0x1) << CSR_GSTINFO_PVM_SHIFT)
#define CSR_GSTINFO_VM_SHIFT	0
#define CSR_GSTINFO_VM		(_ULCAST_(0x1) << CSR_GSTINFO_VM_SHIFT)

#if 0
#define LISA_CSR_GSTCFG		0x51	// 32 guest config
#define CSR_GSTCFG_GPERF_SHIFT	24
#define CSR_GSTCFG_GPERF_WIDTH	3
#define CSR_GSTCFG_GPERF	(_ULCAST_(0x7) << CSR_GSTCFG_GPERF_SHIFT)
#define CSR_GSTCFG_GCI_SHIFT	20
#define CSR_GSTCFG_GCI_WIDTH	2
#define CSR_GSTCFG_GCI		(_ULCAST_(0x3) << CSR_GSTCFG_GCI_SHIFT)
#define CSR_GSTCFG_GCI_ALL	(_ULCAST_(0x0) << CSR_GSTCFG_GCI_SHIFT)
#define CSR_GSTCFG_GCI_HIT	(_ULCAST_(0x1) << CSR_GSTCFG_GCI_SHIFT)
#define CSR_GSTCFG_GCI_SECURE	(_ULCAST_(0x2) << CSR_GSTCFG_GCI_SHIFT)
#define CSR_GSTCFG_GCIP_SHIFT	16
#define CSR_GSTCFG_GCIP		(_ULCAST_(0xf) << CSR_GSTCFG_GCIP_SHIFT)
#define CSR_GSTCFG_GCIP_ALL	(_ULCAST_(0x1) << CSR_GSTCFG_GCIP_SHIFT)
#define CSR_GSTCFG_GCIP_HIT	(_ULCAST_(0x1) << (CSR_GSTCFG_GCIP_SHIFT + 1))
#define CSR_GSTCFG_GCIP_SECURE	(_ULCAST_(0x1) << (CSR_GSTCFG_GCIP_SHIFT + 2))
#define CSR_GSTCFG_TORU_SHIFT	15
#define CSR_GSTCFG_TORU		(_ULCAST_(0x1) << CSR_GSTCFG_TORU_SHIFT)
#define CSR_GSTCFG_TORUP_SHIFT	14
#define CSR_GSTCFG_TORUP	(_ULCAST_(0x1) << CSR_GSTCFG_TORUP_SHIFT)
#define CSR_GSTCFG_TOP_SHIFT	13
#define CSR_GSTCFG_TOP		(_ULCAST_(0x1) << CSR_GSTCFG_TOP_SHIFT)
#define CSR_GSTCFG_TOPP_SHIFT	12
#define CSR_GSTCFG_TOPP		(_ULCAST_(0x1) << CSR_GSTCFG_TOPP_SHIFT)
#define CSR_GSTCFG_TOE_SHIFT	11
#define CSR_GSTCFG_TOE		(_ULCAST_(0x1) << CSR_GSTCFG_TOE_SHIFT)
#define CSR_GSTCFG_TOEP_SHIFT	10
#define CSR_GSTCFG_TOEP		(_ULCAST_(0x1) << CSR_GSTCFG_TOEP_SHIFT)
#define CSR_GSTCFG_TIT_SHIFT	9
#define CSR_GSTCFG_TIT		(_ULCAST_(0x1) << CSR_GSTCFG_TIT_SHIFT)
#define CSR_GSTCFG_TITP_SHIFT	8
#define CSR_GSTCFG_TITP		(_ULCAST_(0x1) << CSR_GSTCFG_TITP_SHIFT)
#define CSR_GSTCFG_SIT_SHIFT	7
#define CSR_GSTCFG_SIT		(_ULCAST_(0x1) << CSR_GSTCFG_SIT_SHIFT)
#define CSR_GSTCFG_SITP_SHIFT	6
#define CSR_GSTCFG_SITP		(_ULCAST_(0x1) << CSR_GSTCFG_SITP_SHIFT)
#define CSR_GSTCFG_CACTRL_SHIFT	4
#define CSR_GSTCFG_CACTRL_WIDTH	2
#define CSR_GSTCFG_CACTRL	(_ULCAST_(0x3) << CSR_GSTCFG_CACTRL_SHIFT)
#define CSR_GSTCFG_CACTRL_GUEST	(_ULCAST_(0x0) << CSR_GSTCFG_CACTRL_SHIFT)
#define CSR_GSTCFG_CACTRL_ROOT	(_ULCAST_(0x1) << CSR_GSTCFG_CACTRL_SHIFT)
#define CSR_GSTCFG_CACTRL_NEST	(_ULCAST_(0x2) << CSR_GSTCFG_CACTRL_SHIFT)
#define CSR_GSTCFG_CCCP_WIDTH	4
#define CSR_GSTCFG_CCCP		_ULCAST_(0xf)
#define CSR_GSTCFG_CCCP_GUEST	(_ULCAST_(0x1) << 0)
#define CSR_GSTCFG_CCCP_ROOT	(_ULCAST_(0x1) << 1)
#define CSR_GSTCFG_CCCP_NEST	(_ULCAST_(0x1) << 2)

#define LISA_CSR_GSTEX		0x52	// 64 guest exception control
#define CSR_GSTEX_HC_SHIFT	16
#define CSR_GSTEX_HC_WIDTH	8
#define CSR_GSTEX_HC		(_ULCAST_(0xff) << CSR_GSTEX_HC_SHIFT)
#define CSR_GSTEX_PIP_SHIFT	8
#define CSR_GSTEX_PIP_WIDTH	8
#define CSR_GSTEX_PIP		(_ULCAST_(0xff) << CSR_GSTEX_PIP_SHIFT)
#define CSR_GSTEX_VIP_SHIFT	0
#define CSR_GSTEX_VIP_WIDTH	8
#define CSR_GSTEX_VIP		(_ULCAST_(0xff))

#define LISA_CSR_GSTTMOFFSET	0x53	// 64 guest timer offset
#endif
/* LLBit */
#define LISA_CSR_LLBIT		0x60	// 32 csr number to be changed
#define CSR_LLBIT_ROLLB_SHIFT	0
#define CSR_LLBIT_ROLLB		(_ULCAST_(1) << CSR_LLBIT_ROLLB_SHIFT)
#define CSR_LLBIT_WCLLB_SHIFT	1
#define CSR_LLBIT_WCLLB		(_ULCAST_(1) << CSR_LLBIT_WCLLB_SHIFT)
#define CSR_LLBIT_KLO_SHIFT	2
#define CSR_LLBIT_KLO		(_ULCAST_(1) << CSR_LLBIT_KLO_SHIFT)
#if 0
/* implement dependent */
#define LISA_CSR_GSCFG		0x80	// 32 loongson config / MIPS CP0 config6
#define CSR_GSCFG_MISPEC_SHIFT	20
#define CSR_GSCFG_MISPEC_WIDTH	8
#define CSR_GSCFG_MISPEC	(_ULCAST_(0xff) << CSR_GSCFG_MISPEC_SHIFT)
#define CSR_GSCFG_SSEN_SHIFT	18
#define CSR_GSCFG_SSEN		(_ULCAST_(1) << CSR_GSCFG_SSEN_SHIFT)
#define CSR_GSCFG_SCRAND_SHIFT	17
#define CSR_GSCFG_SCRAND	(_ULCAST_(1) << CSR_GSCFG_SCRAND_SHIFT)
#define CSR_GSCFG_LLEXCL_SHIFT	16
#define CSR_GSCFG_LLEXCL	(_ULCAST_(1) << CSR_GSCFG_LLEXCL_SHIFT)
#define CSR_GSCFG_DISVC_SHIFT	15
#define CSR_GSCFG_DISVC		(_ULCAST_(1) << CSR_GSCFG_DISVC_SHIFT)
#define CSR_GSCFG_VCLRU_SHIFT	14
#define CSR_GSCFG_VCLRU		(_ULCAST_(1) << CSR_GSCFG_VCLRU_SHIFT)
#define CSR_GSCFG_DCLRU_SHIFT	13
#define CSR_GSCFG_DCLRU		(_ULCAST_(1) << CSR_GSCFG_DCLRU_SHIFT)
#define CSR_GSCFG_FASTLDQ_SHIFT	12
#define CSR_GSCFG_FASTLDQ	(_ULCAST_(1) << CSR_GSCFG_FASTLDQ_SHIFT)
#define CSR_GSCFG_USERCAC_SHIFT	11
#define CSR_GSCFG_USERCAC	(_ULCAST_(1) << CSR_GSCFG_USERCAC_SHIFT)
#define CSR_GSCFG_ANTI_MISPEC_SHIFT	10
#define CSR_GSCFG_ANTI_MISPEC	(_ULCAST_(1) << CSR_GSCFG_ANTI_MISPEC_SHIFT)
#define CSR_GSCFG_ANTI_FLUSHSFB_SHIFT	9
#define CSR_GSCFG_ANTI_FLUSHSFB	(_ULCAST_(1) << CSR_GSCFG_ANTI_FLUSHSFB_SHIFT)
#define CSR_GSCFG_STFILL_SHIFT	8
#define CSR_GSCFG_STFILL	(_ULCAST_(1) << CSR_GSCFG_STFILL_SHIFT)
#define CSR_GSCFG_LIFEP_SHIFT	7
#define CSR_GSCFG_LIFEP		(_ULCAST_(1) << CSR_GSCFG_LIFEP_SHIFT)
#define CSR_GSCFG_LLSYNC_SHIFT	6
#define CSR_GSCFG_LLSYNC	(_ULCAST_(1) << CSR_GSCFG_LLSYNC_SHIFT)
#define CSR_GSCFG_BRBTDIS_SHIFT	5
#define CSR_GSCFG_BRBTDIS	(_ULCAST_(1) << CSR_GSCFG_BRBTDIS_SHIFT)
#define CSR_GSCFG_RASDIS_SHIFT	4
#define CSR_GSCFG_RASDIS	(_ULCAST_(1) << CSR_GSCFG_RASDIS_SHIFT)
#define CSR_GSCFG_STPRE_SHIFT	2
#define CSR_GSCFG_STPRE_WIDTH	2
#define CSR_GSCFG_STPRE		(_ULCAST_(3) << CSR_GSCFG_STPRE_SHIFT)
#define CSR_GSCFG_INSTPRE_SHIFT	1
#define CSR_GSCFG_INSTPRE	(_ULCAST_(1) << CSR_GSCFG_INSTPRE_SHIFT)
#define CSR_GSCFG_DATAPRE_SHIFT	0
#define CSR_GSCFG_DATAPRE	(_ULCAST_(1) << CSR_GSCFG_DATAPRE_SHIFT)
#endif

#define LISA_CSR_FLUSH		0x81	// 32 Flush
#define CSR_FLUSH_VTLB_SHIFT	0
#define CSR_FLUSH_VTLB		(_ULCAST_(1) << CSR_FLUSH_VTLB_SHIFT)
#define CSR_FLUSH_FTLB_SHIFT	1
#define CSR_FLUSH_FTLB		(_ULCAST_(1) << CSR_FLUSH_FTLB_SHIFT)
#define CSR_FLUSH_DTLB_SHIFT	2
#define CSR_FLUSH_DTLB		(_ULCAST_(1) << CSR_FLUSH_DTLB_SHIFT)
#define CSR_FLUSH_ITLB_SHIFT	3
#define CSR_FLUSH_ITLB		(_ULCAST_(1) << CSR_FLUSH_ITLB_SHIFT)
#define CSR_FLUSH_BTAC_SHIFT	4
#define CSR_FLUSH_BTAC		(_ULCAST_(1) << CSR_FLUSH_BTAC_SHIFT)

#define LISA_FLUSH_VTLB		1
#define LISA_FLUSH_FTLB		(1 << 1)
#define LISA_FLUSH_DTLB		(1 << 2)
#define LISA_FLUSH_ITLB		(1 << 3)
#define LISA_FLUSH_BTAC		(1 << 4)

#define LISA_CSR_GNMI		0x82

/* TLB Refill Only */
#define LISA_CSR_RFBASE		0x88	// 64 TLB refill exception address
#define LISA_CSR_RFBADV		0x89	// 64 TLB refill badvaddr
#define LISA_CSR_RFEPC		0x8a	// 64 TLB refill EPC
#define LISA_CSR_RFSCR		0x8b	// 64 KScratch for TLB refill
#define LISA_CSR_RFENLO0	0x8c	// 64 TLB refill entrylo0
#define LISA_CSR_RFENLO1	0x8d	// 64 TLB refill entrylo1
#define LISA_CSR_RFENHI		0x8e	// 64 TLB refill entryhi
#define LISA_CSR_RFSAVE		0x8f	// 64 TLB refill saving info

/* error related */
#define LISA_CSR_ERRCTL		0x90	// 32 ERRCTL
#define LISA_CSR_ERRINFO	0x91	// ??
#define LISA_CSR_ERRINFO1	0x92	// ??
#define LISA_CSR_ERRBASE	0x93	// 64 error exception base
#define LISA_CSR_ERREPC		0x94	// 64 error exception PC
#define LISA_CSR_ERRKS		0x95	// 64 KScratch for error exception

#define LISA_CSR_TAG		0x98	// 64 TagLo + TagHi
#if 0
/* shadow MCSR : 0xc0 ~ 0xff */
#define LISA_CSR_MCSR0		0xc0	// 64 cpucfg0 and 1
#define MCSR0_INT_IMPL_SHIFT	58
#define MCSR0_INT_IMPL		0
#define MCSR0_IOCSR_BRD_SHIFT	57
#define MCSR0_IOCSR_BRD		(_ULCAST_(1) << MCSR0_IOCSR_BRD_SHIFT)
#define MCSR0_HUGEPG_SHIFT	56
#define MCSR0_HUGEPG		(_ULCAST_(1) << MCSR0_HUGEPG_SHIFT)
#define MCSR0_RPLVTLB_SHIFT	55
#define MCSR0_RPLVTLB		(_ULCAST_(1) << MCSR0_RPLVTLB_SHIFT)
#define MCSR0_EXEPROT_SHIFT	54
#define MCSR0_EXEPROT		(_ULCAST_(1) << MCSR0_EXEPROT_SHIFT)
#define MCSR0_RI_SHIFT		53
#define MCSR0_RI		(_ULCAST_(1) << MCSR0_RI_SHIFT)
#define MCSR0_UAL_SHIFT		52
#define MCSR0_UAL		(_ULCAST_(1) << MCSR0_UAL_SHIFT)
#define MCSR0_VABIT_SHIFT	44
#define MCSR0_VABIT_WIDTH	8
#define MCSR0_VABIT		(_ULCAST_(0xff) << MCSR0_VABIT_SHIFT)
#define VABIT_DEFAULT		0x2f
#define MCSR0_PABIT_SHIFT	36
#define MCSR0_PABIT_WIDTH	8
#define MCSR0_PABIT		(_ULCAST_(0xff) << MCSR0_PABIT_SHIFT)
#define PABIT_DEFAULT		0x2f
#define MCSR0_IOCSR_SHIFT	35
#define MCSR0_IOCSR		(_ULCAST_(1) << MCSR0_IOCSR_SHIFT)
#define MCSR0_PAGING_SHIFT	34
#define MCSR0_PAGING		(_ULCAST_(1) << MCSR0_PAGING_SHIFT)
#define MCSR0_GR64_SHIFT	33
#define MCSR0_GR64		(_ULCAST_(1) << MCSR0_GR64_SHIFT)
#define GR64_DEFAULT		1
#define MCSR0_GR32_SHIFT	32
#define MCSR0_GR32		(_ULCAST_(1) << MCSR0_GR32_SHIFT)
#define GR32_DEFAULT		0
#define MCSR0_PRID_WIDTH	32
#define MCSR0_PRID		0X14C010

#define LISA_CSR_MCSR1		0xc1	// 64 cpucfg2 and 3
#define MCSR1_HPFOLD_SHIFT	43
#define MCSR1_HPFOLD		(_ULCAST_(1) << MCSR1_HPFOLD_SHIFT)
#define MCSR1_SPW_LVL_SHIFT	40
#define MCSR1_SPW_LVL_WIDTH	3
#define MCSR1_SPW_LVL		(_ULCAST_(7) << MCSR1_SPW_LVL_SHIFT)
#define MCSR1_ICACHET_SHIFT	39
#define MCSR1_ICACHET		(_ULCAST_(1) << MCSR1_ICACHET_SHIFT)
#define MCSR1_ITLBT_SHIFT	38
#define MCSR1_ITLBT		(_ULCAST_(1) << MCSR1_ITLBT_SHIFT)
#define MCSR1_LLDBAR_SHIFT	37
#define MCSR1_LLDBAR		(_ULCAST_(1) << MCSR1_LLDBAR_SHIFT)
#define MCSR1_SCDLY_SHIFT	36
#define MCSR1_SCDLY		(_ULCAST_(1) << MCSR1_SCDLY_SHIFT)
#define MCSR1_LLEXC_SHIFT	35
#define MCSR1_LLEXC		(_ULCAST_(1) << MCSR1_LLEXC_SHIFT)
#define MCSR1_UCACC_SHIFT	34
#define MCSR1_UCACC		(_ULCAST_(1) << MCSR1_UCACC_SHIFT)
#define MCSR1_SFB_SHIFT		33
#define MCSR1_SFB		(_ULCAST_(1) << MCSR1_SFB_SHIFT)
#define MCSR1_CCDMA_SHIFT	32
#define MCSR1_CCDMA		(_ULCAST_(1) << MCSR1_CCDMA_SHIFT)
#define MCSR1_LAMO_SHIFT	22
#define MCSR1_LAMO		(_ULCAST_(1) << MCSR1_LAMO_SHIFT)
#define MCSR1_LSPW_SHIFT	21
#define MCSR1_LSPW		(_ULCAST_(1) << MCSR1_LSPW_SHIFT)
#define MCSR1_MIPSBT_SHIFT	20
#define MCSR1_MIPSBT		(_ULCAST_(1) << MCSR1_MIPSBT_SHIFT)
#define MCSR1_ARMBT_SHIFT	19
#define MCSR1_ARMBT		(_ULCAST_(1) << MCSR1_ARMBT_SHIFT)
#define MCSR1_X86BT_SHIFT	18
#define MCSR1_X86BT		(_ULCAST_(1) << MCSR1_X86BT_SHIFT)
#define MCSR1_LLFTPVERS_SHIFT	15
#define MCSR1_LLFTPVERS_WIDTH	3
#define MCSR1_LLFTPVERS		(_ULCAST_(7) << MCSR1_LLFTPVERS_SHIFT)
#define MCSR1_LLFTP_SHIFT	14
#define MCSR1_LLFTP		(_ULCAST_(1) << MCSR1_LLFTP_SHIFT)
#define MCSR1_VZVERS_SHIFT	11
#define MCSR1_VZVERS_WIDTH	3
#define MCSR1_VZVERS		(_ULCAST_(7) << MCSR1_VZVERS_SHIFT)
#define MCSR1_VZ_SHIFT		10
#define MCSR1_VZ		(_ULCAST_(1) << MCSR1_VZ_SHIFT)
#define MCSR1_CRYPTO_SHIFT	9
#define MCSR1_CRYPTO		(_ULCAST_(1) << MCSR1_CRYPTO_SHIFT)
#define MCSR1_COMPLEX_SHIFT	8
#define MCSR1_COMPLEX		(_ULCAST_(1) << MCSR1_COMPLEX_SHIFT)
#define MCSR1_LASX_SHIFT	7
#define MCSR1_LASX		(_ULCAST_(1) << MCSR1_LASX_SHIFT)
#define MCSR1_LSX_SHIFT		6
#define MCSR1_LSX		(_ULCAST_(1) << MCSR1_LSX_SHIFT)
#define MCSR1_FPVERS_SHIFT	3
#define MCSR1_FPVERS_WIDTH	3
#define MCSR1_FPVERS		(_ULCAST_(7) << MCSR1_FPVERS_SHIFT)
#define MCSR1_FPDP_SHIFT	2
#define MCSR1_FPDP		(_ULCAST_(1) << MCSR1_FPDP_SHIFT)
#define MCSR1_FPSP_SHIFT	1
#define MCSR1_FPSP		(_ULCAST_(1) << MCSR1_FPSP_SHIFT)
#define MCSR1_FP_SHIFT		0
#define MCSR1_FP		(_ULCAST_(1) << MCSR1_FP_SHIFT)

#define LISA_CSR_MCSR2		0xc2	// 64 cpucfg4 and 5
#define MCSR2_CCDIV_SHIFT	48
#define MCSR2_CCDIV_WIDTH	16
#define MCSR2_CCDIV		(_ULCAST_(0xffff) << MCSR2_CCDIV_SHIFT)
#define MCSR2_CCMUL_SHIFT	32
#define MCSR2_CCMUL_WIDTH	16
#define MCSR2_CCMUL		(_ULCAST_(0xffff) << MCSR2_CCMUL_SHIFT)
#define MCSR2_CCFREQ_WIDTH	32
#define MCSR2_CCFREQ		(_ULCAST_(0xffffffff)
#define CCFREQ_DEFAULT		0x5f5e100	// 100MHZ

#define LISA_CSR_MCSR3		0xc3	// 64 cpucfg6
#define MCSR3_UPM_SHIFT		14
#define MCSR3_UPM		(_ULCAST_(1) << MCSR3_UPM_SHIFT)
#define MCSR3_PMBITS_SHIFT	8
#define MCSR3_PMBITS_WIDTH	6
#define MCSR3_PMBITS		(_ULCAST_(0x3f) << MCSR3_PMBITS_SHIFT)
#define PMBITS_DEFAULT		0x40
#define MCSR3_PMNUM_SHIFT	4
#define MCSR3_PMNUM_WIDTH	4
#define MCSR3_PMNUM		(_ULCAST_(0xf) << MCSR3_PMNUM_SHIFT)
#define MCSR3_PAMVER_SHIFT	1
#define MCSR3_PAMVER_WIDTH	3
#define MCSR3_PAMVER		(_ULCAST_(0x7) << MCSR3_PAMVER_SHIFT)
#define MCSR3_PMP_SHIFT		0
#define MCSR3_PMP		(_ULCAST_(1) << MCSR3_PMP_SHIFT)

#define LISA_CSR_MCSR8		0xc8	// 64 cpucfg10 and 11
#define MCSR8_L1I_SIZE_SHIFT	56
#define MCSR8_L1I_SIZE_WIDTH	7
#define MCSR8_L1I_SIZE		(_ULCAST_(0x7f) << MCSR8_L1I_SIZE_SHIFT)
#define MCSR8_L1I_IDX_SHIFT	48
#define MCSR8_L1I_IDX_WIDTH	8
#define MCSR8_L1I_IDX		(_ULCAST_(0xff) << MCSR8_L1I_IDX_SHIFT)
#define MCSR8_L1I_WAY_SHIFT	32
#define MCSR8_L1I_WAY_WIDTH	16
#define MCSR8_L1I_WAY		(_ULCAST_(0xffff) << MCSR8_L1I_WAY_SHIFT)
#define MCSR8_L3DINCL_SHIFT	16
#define MCSR8_L3DINCL		(_ULCAST_(1) << MCSR8_L3DINCL_SHIFT)
#define MCSR8_L3DPRIV_SHIFT	15
#define MCSR8_L3DPRIV		(_ULCAST_(1) << MCSR8_L3DPRIV_SHIFT)
#define MCSR8_L3DPRE_SHIFT	14
#define MCSR8_L3DPRE		(_ULCAST_(1) << MCSR8_L3DPRE_SHIFT)
#define MCSR8_L3IUINCL_SHIFT	13
#define MCSR8_L3IUINCL		(_ULCAST_(1) << MCSR8_L3IUINCL_SHIFT)
#define MCSR8_L3IUPRIV_SHIFT	12
#define MCSR8_L3IUPRIV		(_ULCAST_(1) << MCSR8_L3IUPRIV_SHIFT)
#define MCSR8_L3IUUNIFY_SHIFT	11
#define MCSR8_L3IUUNIFY		(_ULCAST_(1) << MCSR8_L3IUUNIFY_SHIFT)
#define MCSR8_L3IUPRE_SHIFT	10
#define MCSR8_L3IUPRE		(_ULCAST_(1) << MCSR8_L3IUPRE_SHIFT)
#define MCSR8_L2DINCL_SHIFT	9
#define MCSR8_L2DINCL		(_ULCAST_(1) << MCSR8_L2DINCL_SHIFT)
#define MCSR8_L2DPRIV_SHIFT	8
#define MCSR8_L2DPRIV		(_ULCAST_(1) << MCSR8_L2DPRIV_SHIFT)
#define MCSR8_L2DPRE_SHIFT	7
#define MCSR8_L2DPRE		(_ULCAST_(1) << MCSR8_L2DPRE_SHIFT)
#define MCSR8_L2IUINCL_SHIFT	6
#define MCSR8_L2IUINCL		(_ULCAST_(1) << MCSR8_L2IUINCL_SHIFT)
#define MCSR8_L2IUPRIV_SHIFT	5
#define MCSR8_L2IUPRIV		(_ULCAST_(1) << MCSR8_L2IUPRIV_SHIFT)
#define MCSR8_L2IUUNIFY_SHIFT	4
#define MCSR8_L2IUUNIFY		(_ULCAST_(1) << MCSR8_L2IUUNIFY_SHIFT)
#define MCSR8_L2IUPRE_SHIFT	3
#define MCSR8_L2IUPRE		(_ULCAST_(1) << MCSR8_L2IUPRE_SHIFT)
#define MCSR8_L1DPRE_SHIFT	2
#define MCSR8_L1DPRE		(_ULCAST_(1) << MCSR8_L1DPRE_SHIFT)
#define MCSR8_L1IUUNIFY_SHIFT	1
#define MCSR8_L1IUUNIFY		(_ULCAST_(1) << MCSR8_L1IUUNIFY_SHIFT)
#define MCSR8_L1IUPRE_SHIFT	0
#define MCSR8_L1IUPRE		(_ULCAST_(1) << MCSR8_L1IUPRE_SHIFT)

#define LISA_CSR_MCSR9		0xc9	// 64 cpucfg12 and 13
#define MCSR9_L2U_SIZE_SHIFT	56
#define MCSR9_L2U_SIZE_WIDTH	7
#define MCSR9_L2U_SIZE		(_ULCAST_(0x7f) << MCSR9_L2U_SIZE_SHIFT)
#define MCSR9_L2U_IDX_SHIFT	48
#define MCSR9_L2U_IDX_WIDTH	8
#define MCSR9_L2U_IDX		(_ULCAST_(0xff) << MCSR9_IDX_LOG_SHIFT)
#define MCSR9_L2U_WAY_SHIFT	32
#define MCSR9_L2U_WAY_WIDTH	16
#define MCSR9_L2U_WAY		(_ULCAST_(0xffff) << MCSR9_L2U_WAY_SHIFT)
#define MCSR9_L1D_SIZE_SHIFT	24
#define MCSR9_L1D_SIZE_WIDTH	7
#define MCSR9_L1D_SIZE		(_ULCAST_(0x7f) << MCSR9_L1D_SIZE_SHIFT)
#define MCSR9_L1D_IDX_SHIFT	16
#define MCSR9_L1D_IDX_WIDTH	8
#define MCSR9_L1D_IDX		(_ULCAST_(0xff) << MCSR9_L1D_IDX_SHIFT)
#define MCSR9_L1D_WAY_SHIFT	0
#define MCSR9_L1D_WAY_WIDTH	16
#define MCSR9_L1D_WAY		(_ULCAST_(0xffff) << MCSR9_L1D_WAY_SHIFT)

#define LISA_CSR_MCSRX		0xca	// 64 cpucfg14
#define MCSRX_L3U_SIZE_SHIFT	24
#define MCSRX_L3U_SIZE_WIDTH	7
#define MCSRX_L3U_SIZE		(_ULCAST_(0x7f) << MCSRX_L3U_SIZE_SHIFT)
#define MCSRX_L3U_IDX_SHIFT	16
#define MCSRX_L3U_IDX_WIDTH	8
#define MCSRX_L3U_IDX		(_ULCAST_(0xff) << MCSRX_L3U_IDX_SHIFT)
#define MCSRX_L3U_WAY_SHIFT	0
#define MCSRX_L3U_WAY_WIDTH	16
#define MCSRX_L3U_WAY		(_ULCAST_(0xffff) << MCSRX_L3U_WAY_SHIFT)

#define LISA_CSR_MCSRXXX	0xf0	// 64 cpucfg30
#define MCSRXXX_RAMCG_SHIFT	3
#define MCSRXXX_RAMCG		(_ULCAST_(1) << MCSRXXX_RAMCG_SHIFT)
#define MCSRXXX_VFPUCG_SHIFT	2
#define MCSRXXX_VFPUCG		(_ULCAST_(1) << MCSRXXX_VFPUCG_SHIFT)
#define MCSRXXX_NAPEN_SHIFT	1
#define MCSRXXX_NAPEN		(_ULCAST_(1) << MCSRXXX_NAPEN_SHIFT)
#define MCSRXXX_MCSRLOCK_SHIFT	0
#define MCSRXXX_MCSRLOCK	(_ULCAST_(1) << MCSRXXX_MCSRLOCK_SHIFT)
#endif
#if 0  /* La32 unsupport UCA for temporary */
/* uncached accelerate windows */
#define LISA_CSR_UCAWIN		0x100	// ?? read only info
#define LISA_CSR_UCAWIN0_LO	0x102	// ?? win0 low
#define LISA_CSR_UCAWIN0_HI	0x103	// ?? win0 high
#define LISA_CSR_UCAWIN1_LO	0x104	// ?? win1 low
#define LISA_CSR_UCAWIN1_HI	0x105	// ?? win1 high
#define LISA_CSR_UCAWIN2_LO	0x106	// ?? win2 low
#define LISA_CSR_UCAWIN2_HI	0x107	// ?? win2 high
#define LISA_CSR_UCAWIN3_LO	0x108	// ?? win3 low
#define LISA_CSR_UCAWIN3_HI	0x109	// ?? win3 high
#endif
/* direct map windows */
#define LISA_CSR_DMWIN0		0x180	// 64 direct map win0: MEM & IF
#define LISA_CSR_DMWIN1		0x181	// 64 direct map win1: MEM & IF
#define LISA_CSR_DMWIN2		0x182	// 64 direct map win2: MEM
#define LISA_CSR_DMWIN3		0x183	// 64 direct map win3: MEM
#if 0 /* La32 unsupport PERF for temporary */
/* performance counter */
#define LISA_CSR_PERFCTRL0	0x200	// 32 perf event 0 config
#define LISA_CSR_PERFCNTR0	0x201	// 64 perf event 0 count value
#define LISA_CSR_PERFCTRL1	0x202	// 32 perf event 1 config
#define LISA_CSR_PERFCNTR1	0x203	// 64 perf event 1 count value
#define LISA_CSR_PERFCTRL2	0x204	// 32 perf event 2 config
#define LISA_CSR_PERFCNTR2	0x205	// 64 perf event 2 count value
#define LISA_CSR_PERFCTRL3	0x206	// 32 perf event 3 config
#define LISA_CSR_PERFCNTR3	0x207	// 64 perf event 3 count value
#define CSR_PERFCTRL_PLV0	(_ULCAST_(1) << 16)
#define CSR_PERFCTRL_PLV1	(_ULCAST_(1) << 17)
#define CSR_PERFCTRL_PLV2	(_ULCAST_(1) << 18)
#define CSR_PERFCTRL_PLV3	(_ULCAST_(1) << 19)
#define CSR_PERFCTRL_IE		(_ULCAST_(1) << 20)
#define CSR_PERFCTRL_EVENT	0x3ff
#endif
#if 0 /* La32 unsupport DEBUG for temporary */
/* debug */
#define LISA_CSR_DBCFG		0x300	// ?? debug breakpoint config
#define LISA_CSR_DBST		0x301	// ?? debug breakpoint status

#define LISA_CSR_DB0ADDR	0x310	// 64 debug breakpoint 0 address
#define LISA_CSR_DB0MASK	0x311	// 64 debug breakpoint 0 mask
#define LISA_CSR_DB0CTL		0x312	// ?? debug breakpoint 0 control
#define LISA_CSR_DB0ASID	0x313	// ?? debug breakpoint 0 asid

#define LISA_CSR_DB1ADDR	0x318	// 64 debug breakpoint 1 address
#define LISA_CSR_DB1MASK	0x319	// 64 debug breakpoint 1 mask
#define LISA_CSR_DB1CTL		0x31a	// ?? debug breakpoint 1 control
#define LISA_CSR_DB1ASID	0x31b	// ?? debug breakpoint 1 asid

#define LISA_CSR_DB2ADDR	0x320	// 64 debug breakpoint 2 address
#define LISA_CSR_DB2MASK	0x321	// 64 debug breakpoint 2 mask
#define LISA_CSR_DB2CTL		0x322	// ?? debug breakpoint 2 control
#define LISA_CSR_DB2ASID	0x323	// ?? debug breakpoint 2 asid

#define LISA_CSR_DB3ADDR	0x328	// 64 debug breakpoint 3 address
#define LISA_CSR_DB3MASK	0x329	// 64 debug breakpoint 3 mask
#define LISA_CSR_DB3CTL		0x32a	// ?? debug breakpoint 3 control
#define LISA_CSR_DB3ASID	0x32b	// ?? debug breakpoint 3 asid

#define LISA_CSR_IBCFG		0x380	// ?? instruction breakpoint config
#define LISA_CSR_IBST		0x381	// ?? instruction breakpoint status

#define LISA_CSR_IB0ADDR	0x390	// 64 inst breakpoint 0 address
#define LISA_CSR_IB0MASK	0x391	// 64 inst breakpoint 0 mask
#define LISA_CSR_IB0CTL		0x392	// ?? inst breakpoint 0 control
#define LISA_CSR_IB0ASID	0x393	// ?? inst breakpoint 0 asid

#define LISA_CSR_IB1ADDR	0x398	// 64 inst breakpoint 1 address
#define LISA_CSR_IB1MASK	0x399	// 64 inst breakpoint 1 mask
#define LISA_CSR_IB1CTL		0x39a	// ?? inst breakpoint 1 control
#define LISA_CSR_IB1ASID	0x39b	// ?? inst breakpoint 1 asid

#define LISA_CSR_IB2ADDR	0x3a0	// 64 inst breakpoint 2 address
#define LISA_CSR_IB2MASK	0x3a1	// 64 inst breakpoint 2 mask
#define LISA_CSR_IB2CTL		0x3a2	// ?? inst breakpoint 2 control
#define LISA_CSR_IB2ASID	0x3a3	// ?? inst breakpoint 2 asid

#define LISA_CSR_IB3ADDR	0x3a8	// 64 inst breakpoint 3 address
#define LISA_CSR_IB3MASK	0x3a9	// 64 inst breakpoint 3 mask
#define LISA_CSR_IB3CTL		0x3aa	// ?? inst breakpoint 3 control
#define LISA_CSR_IB3ASID	0x3ab	// ?? inst breakpoint 3 asid

#define LISA_CSR_IB4ADDR	0x3b0	// 64 inst breakpoint 4 address
#define LISA_CSR_IB4MASK	0x3b1	// 64 inst breakpoint 4 mask
#define LISA_CSR_IB4CTL		0x3b2	// ?? inst breakpoint 4 control
#define LISA_CSR_IB4ASID	0x3b3	// ?? inst breakpoint 4 asid

#define LISA_CSR_IB5ADDR	0x3b8	// 64 inst breakpoint 5 address
#define LISA_CSR_IB5MASK	0x3b9	// 64 inst breakpoint 5 mask
#define LISA_CSR_IB5CTL		0x3ba	// ?? inst breakpoint 5 control
#define LISA_CSR_IB5ASID	0x3bb	// ?? inst breakpoint 5 asid

#define LISA_CSR_IB6ADDR	0x3c0	// 64 inst breakpoint 6 address
#define LISA_CSR_IB6MASK	0x3c1	// 64 inst breakpoint 6 mask
#define LISA_CSR_IB6CTL		0x3c2	// ?? inst breakpoint 6 control
#define LISA_CSR_IB6ASID	0x3c3	// ?? inst breakpoint 6 asid

#define LISA_CSR_IB7ADDR	0x3c8	// 64 inst breakpoint 7 address
#define LISA_CSR_IB7MASK	0x3c9	// 64 inst breakpoint 7 mask
#define LISA_CSR_IB7CTL		0x3ca	// ?? inst breakpoint 7 control
#define LISA_CSR_IB7ASID	0x3cb	// ?? inst breakpoint 7 asid

#define LISA_CSR_DEBUG		0x500	// 32 like EJTAG debug
#define LISA_CSR_DESAVE		0x501	// ??
#define LISA_CSR_DEPC		0x502	// 64

#endif
#define LISA_CSR_PRID		0xc0	// 32 MIPS CP0 PRID
/* Bitmap of csr registers */

/*
   (CSR 0x28) is defined in this (backwards compatible) way:

  +----------------+----------------+----------------+----------------+
  | Company Options| Company ID	    | Processor ID   | Revision	      |
  +----------------+----------------+----------------+----------------+
   31		 24 23		  16 15		    8 7
*/

/*
 * Assigned Company Options for bits 31:24 of the PRId register.
 */
//#define PRID_OPT_MASK           0xff000000
#define PRID_COMP_LOONGSON	0x140000

/*
 * Assigned Company values for bits 23:16 of the PRId register.
 */
//#define PRID_COMP_MASK          0xff0000
#define PRID_REV_LOONGSON	0xc0000

/*
 * Assigned Processor ID (implementation) values for bits 15:8 of the PRId
 * register.  In order to detect a certain CPU type exactly eventually
 * additional registers may need to be examined.
 */
//#define PRID_IMP_MASK           0xff00
#define PRID_IMP_LOONGISA	0x6300

/*
 * Definitions for 7:0 on legacy processors
 */
//#define PRID_REV_MASK           0xff
#define PRID_REV_LOONGISA	0x1

/*
 * CSR_ExConfig IM
 */
#define ECFG0_IM		0x00001fff
#define ECFGB_SIP0		0
#define ECFGF_SIP0		(_ULCAST_(1) << ECFGB_SIP0)
#define ECFGB_SIP1		1
#define ECFGF_SIP1		(_ULCAST_(1) << ECFGB_SIP1)
#define ECFGB_IP0		2
#define ECFGF_IP0		(_ULCAST_(1) << ECFGB_IP0)
#define ECFGB_IP1		3
#define ECFGF_IP1		(_ULCAST_(1) << ECFGB_IP1)
#define ECFGB_IP2		4
#define ECFGF_IP2		(_ULCAST_(1) << ECFGB_IP2)
#define ECFGB_IP3		5
#define ECFGF_IP3		(_ULCAST_(1) << ECFGB_IP3)
#define ECFGB_IP4		6
#define ECFGF_IP4		(_ULCAST_(1) << ECFGB_IP4)
#define ECFGB_IP5		7
#define ECFGF_IP5		(_ULCAST_(1) << ECFGB_IP5)
#define ECFGB_IP6		8
#define ECFGF_IP6		(_ULCAST_(1) << ECFGB_IP6)
#define ECFGB_IP7		9
#define ECFGF_IP7		(_ULCAST_(1) << ECFGB_IP7)
#define ECFGB_PC		10
#define ECFGF_PC		(_ULCAST_(1) << ECFGB_PC)
#define ECFGB_TIMER		11
#define ECFGF_TIMER		(_ULCAST_(1) << ECFGB_TIMER)
#define ECFGB_IPI		12
#define ECFGF_IPI		(_ULCAST_(1) << ECFGB_IPI)
#define ECFGF(hwirq)		(_ULCAST_(1) << (hwirq))

#define ESTF_IP			0x00001fff

#define LISA_EHINV_SH		31
#define LISA_TLBIDX_EHINV	(_ULCAST_(1) << 31)
#define LISA_TLBIDX_PGSIZE	24
#define LISA_TLBIDXM_PGSIZE	0x3f000000
#define CSR_INVALID_ENTRY(e)	(LISA_TLBIDX_EHINV | e)

/*
 * (CSR 0x0 CRMD) is defined as
 * Privilege (bit 1:0)
 * Intterupt enabled (bit 2)
 * Direct address access, exclusive witch mapped address (bit 3)
 * Mapped address access, exclusive witch direct address (bit 4)
 * Instruction cachable Attribute in direct address access mode (bit 6:5)
 * Data cacheable Attribute in direct address access mode (bit 8:7)
 */
#define LISA_CSR_CRMD_PLV		0x3
#define LISA_CSR_CRMD_IE		0x4
#define LISA_CSR_CRMD_DIRECT_ADDR	0x8
#define LISA_CSR_CRMD_MAPPED_ADDR	0x10
#define LISA_CSR_CRMD_I_CACHE_ATTR	0x60
#define LISA_CSR_CRMD_D_CACHE_ATTR	0x180

#define LISA_CSR_TMCFG_EN	(_ULCAST_(1) << 0)
#define LISA_CSR_TMCFG_PERIOD	(_ULCAST_(1) << 1)
#define LISA_CSR_TMCFG_TIMEVAL	(_ULCAST_(0x3fffffffffff) << 2)
#if 0
/* mips register */
#define REG_AT	0x1
#define REG_V0	0x2
#define REG_V1	0x3
#define REG_A0	0x4
#define REG_A1	0x5
#define REG_A2	0x6
#define REG_A3	0x7
#define REG_A4	0x8
#define REG_A5	0x9
#define REG_A6	0xa
#define REG_A7	0xb
#define REG_T0	0xc
#define REG_T1	0xd
#define REG_T2	0xe
#define REG_T3	0xf
#define REG_S0	0x10
#define REG_S1	0x11
#define REG_S2	0x12
#define REG_S3	0x13
#define REG_S4	0x14
#define REG_S5	0x15
#define REG_S6	0x16
#define REG_S7	0x17
#define REG_T8	0x18
#define REG_T9	0x19
#define REG_K0	0x1a
#define REG_K1	0x1b
#define REG_GP	0x1c
#define REG_SP	0x1d
#define REG_FP	0x1e
#define REG_RA	0x1f
#endif
#ifdef __ASSEMBLY__

/*
 * CSR instruction binary code
 *
 * 31:26    opcode                  111011  0x3b
 * 25:22    opcode minor            0x0: csr-32
 *                                  0x1: csr-64
 * 21:10    CSR register number     2^12 = 4096
 * 09:05    rj
 * 04:00    rd
 */

.macro lisa_gen_csr_rdl csr rd
	csrrd	\rd, \csr
.endm

.macro lisa_gen_csr_wrl csr rd
	csrwr	\rd, \csr
.endm

/* rj is mask for rd which write into register csr */
.macro lisa_gen_csr_xchgl csr rj rd
	csrxchg	\rd, \rj, \csr
.endm

.macro lisa_gen_csr_rdq csr rd
	csrrd	\rd, \csr
.endm

.macro lisa_gen_csr_wrq csr rd
	csrwr	\rd, \csr
.endm
#else  /* !__ASSEMBLY__ */

/* Assembly instruction format:	rd, ui14.  */
/* Data types in instruction templates:  USI, USI.  */
#define __csrrd(/*ui14*/ _1) ((unsigned int) __builtin_loongarch_csrrd ((_1)))

/* Assembly instruction format:	rd, ui14.  */
/* Data types in instruction templates:  USI, USI, USI.  */
#define __csrwr(/*unsigned int*/ _1, /*ui14*/ _2) \
  ((unsigned int) __builtin_loongarch_csrwr ((unsigned int) (_1), (_2)))

/* Assembly instruction format:	rd, rj, ui14.  */
/* Data types in instruction templates:  USI, USI, USI, USI.  */
#define __csrxchg(/*unsigned int*/ _1, /*unsigned int*/ _2, /*ui14*/ _3) \
  ((unsigned int) __builtin_loongarch_csrxchg ((unsigned int) (_1), \
					       (unsigned int) (_2), (_3)))

void panic(const char *fmt, ...);

static inline void write_csr_index(unsigned idx) {
	__csrxchg(idx, CSR_TLBIDX_IDXM, LISA_CSR_TLBIDX);
}

static inline void write_csr_pagesize(unsigned size) {
	__csrxchg(size << CSR_TLBIDX_SIZE, CSR_TLBIDX_SIZEM, LISA_CSR_TLBIDX);
}

static inline unsigned read_csr_pagesize(void) {
	return (__csrrd(LISA_CSR_TLBIDX) & CSR_TLBIDX_SIZEM) >> CSR_TLBIDX_SIZE;
}

static inline unsigned mask2size(unsigned mask) {
	panic("'%s' unfinished", __func__);
	return 0;
}

#define read_csr_crmd()			__csrrd(LISA_CSR_CRMD)
#define write_csr_crmd(val)		__csrwr(val, LISA_CSR_CRMD)
#define read_csr_asid()			__csrrd(LISA_CSR_ASID)
#define write_csr_asid(val)		__csrwr(val, LISA_CSR_ASID)
#define read_csr_entrylo0()		__csrrd(LISA_CSR_TLBELO0)
#define write_csr_entrylo0(val)		__csrwr(val, LISA_CSR_TLBELO0)
#define read_csr_entrylo1()		__csrrd(LISA_CSR_TLBELO1)
#define write_csr_entrylo1(val)		__csrwr(val, LISA_CSR_TLBELO1)
#define read_csr_entryhi()		__csrrd(LISA_CSR_TLBEHI)
#define write_csr_entryhi(val)		__csrwr(val, LISA_CSR_TLBEHI)
#define read_csr_exst()			__csrrd(LISA_CSR_EXST)
#define write_csr_exst(val)		__csrwr(val, LISA_CSR_EXST)
#define read_csr_excfg()		__csrrd(LISA_CSR_EXCFG)
#define write_csr_excfg(val)		__csrwr(val, LISA_CSR_EXCFG)
#define read_csr_tlbidx()		__csrrd(LISA_CSR_TLBIDX)
#define write_csr_tlbidx(val)		__csrwr(val, LISA_CSR_TLBIDX)
#define write_csr_flush(val)		__csrwr(val, LISA_CSR_FLUSH)
#define read_csr_cu()			__csrrd(LISA_CSR_CU)
#define write_csr_cu(val)		__csrwr(val, LISA_CSR_CU)
#define read_csr_gscfg()		__csrrd(LISA_CSR_GSCFG)
#define write_csr_gscfg(val)		__csrwr(val, LISA_CSR_GSCFG)
#define read_csr_config1()		__csrrd(LISA_CSR_CONFIG1)
#define write_csr_config1(val)		__csrwr(val, LISA_CSR_CONFIG1)
#define read_csr_config2()		__csrrd(LISA_CSR_CONFIG2)
#define write_csr_config2(val)		__csrwr(val, LISA_CSR_CONFIG2)
#define read_csr_config3()		__csrrd(LISA_CSR_CONFIG3)
#define write_csr_config3(val)		__csrwr(val, LISA_CSR_CONFIG3)
#define read_csr_ftlbpgsize()		__csrrd(LISA_CSR_FTLBPGSIZE)
#define write_csr_ftlbpgsize(val)	__csrwr(val, LISA_CSR_FTLBPGSIZE)
#define read_csr_reduceva()		__csrrd(LISA_CSR_REDUCEVA)
#define write_csr_reduceva(val)		__csrwr(val, LISA_CSR_REDUCEVA)
#define write_csr_tmintclear(val)	__csrwr(val, LISA_CSR_TMINTCLR)

#define read_csr_perfctrl0()		__csrrd(LISA_CSR_PERFCTRL0)
#define read_csr_perfcntr0()		__csrrd(LISA_CSR_PERFCNTR0)
#define read_csr_perfctrl1()		__csrrd(LISA_CSR_PERFCTRL1)
#define read_csr_perfcntr1()		__csrrd(LISA_CSR_PERFCNTR1)
#define read_csr_perfctrl2()		__csrrd(LISA_CSR_PERFCTRL2)
#define read_csr_perfcntr2()		__csrrd(LISA_CSR_PERFCNTR2)
#define read_csr_perfctrl3()		__csrrd(LISA_CSR_PERFCTRL3)
#define read_csr_perfcntr3()		__csrrd(LISA_CSR_PERFCNTR3)
#define write_csr_perfctrl0(val)	__csrwr(val, LISA_CSR_PERFCTRL0)
#define write_csr_perfcntr0(val)	__csrwr(val, LISA_CSR_PERFCNTR0)
#define write_csr_perfctrl1(val)	__csrwr(val, LISA_CSR_PERFCTRL1)
#define write_csr_perfcntr1(val)	__csrwr(val, LISA_CSR_PERFCNTR1)
#define write_csr_perfctrl2(val)	__csrwr(val, LISA_CSR_PERFCTRL2)
#define write_csr_perfcntr2(val)	__csrwr(val, LISA_CSR_PERFCNTR2)
#define write_csr_perfctrl3(val)	__csrwr(val, LISA_CSR_PERFCTRL3)
#define write_csr_perfcntr3(val)	__csrwr(val, LISA_CSR_PERFCNTR3)

/*
 * Manipulate bits in a register.
 */
#define __BUILD_CSR_COMMON(name)				\
static inline unsigned long					\
set_##name(unsigned long set)					\
{								\
	unsigned long res, new;					\
								\
	res = read_##name();					\
	new = res | set;					\
	write_##name(new);					\
								\
	return res;						\
}								\
								\
static inline unsigned long					\
clear_##name(unsigned long clear)				\
{								\
	unsigned long res, new;					\
								\
	res = read_##name();					\
	new = res & ~clear;					\
	write_##name(new);					\
								\
	return res;						\
}								\
								\
static inline unsigned long					\
change_##name(unsigned long change, unsigned long val)		\
{								\
	unsigned long res, new;					\
								\
	res = read_##name();					\
	new = res & ~change;					\
	new |= (val & change);					\
	write_##name(new);					\
								\
	return res;						\
}

#define __BUILD_CSR_OP(name)	__BUILD_CSR_COMMON(csr_##name)

__BUILD_CSR_OP(excfg)
__BUILD_CSR_OP(tlbidx)
__BUILD_CSR_OP(cu)

#define set_csr_exst(val)	\
	__csrxchg(val, val, LISA_CSR_EXST)
#define clear_csr_exst(val)	\
	__csrxchg(~(val), (val), LISA_CSR_EXST)

#endif /* __ASSEMBLY__ */

#endif
