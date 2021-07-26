/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994, 95, 96, 99, 2001 Ralf Baechle
 * Copyright (C) 1994, 1995, 1996 Paul M. Antoine.
 * Copyright (C) 1999 Silicon Graphics, Inc.
 * Copyright (C) 2007  Maciej W. Rozycki
 */
#ifndef _ASM_STACKFRAME_H
#define _ASM_STACKFRAME_H

#include <asm/asm.h>
#include <asm/asmmacro.h>
#include <asm/mipsregs.h>
#include <asm/asm-offsets.h>
#include <asm/thread_info.h>
#include <loongisa_csr.h>

/* Make the addition of cfi info a little easier. */
	.macro cfi_rel_offset reg offset=0 docfi=0
	.if \docfi
	.cfi_rel_offset \reg, \offset
	.endif
	.endm

	.macro cfi_st reg offset=0 docfi=0
	LONG_S	\reg, sp, \offset
	//cfi_rel_offset \reg, \offset, \docfi
	.endm

	.macro cfi_restore reg offset=0 docfi=0
	.if \docfi
	.cfi_restore \reg
	.endif
	.endm

	.macro cfi_ld reg offset=0 docfi=0
	LONG_L	\reg, sp, \offset
	//cfi_restore \reg \offset \docfi
	.endm

		.macro	SAVE_TEMP docfi=0
		csrrd   $r12, LISA_CSR_KS0
		csrrd   $r13, LISA_CSR_KS1
		cfi_st	$r12, PT_R12, \docfi
		cfi_st	$r13, PT_R13, \docfi
		cfi_st	$r14, PT_R14, \docfi
		cfi_st	$r15, PT_R15, \docfi
		cfi_st	$r16, PT_R16, \docfi
		cfi_st	$r17, PT_R17, \docfi
		cfi_st	$r18, PT_R18, \docfi
		cfi_st	$r19, PT_R19, \docfi
		cfi_st	$r20, PT_R20, \docfi
		.endm

		.macro	SAVE_STATIC docfi=0
		cfi_st	$r23, PT_R23, \docfi
		cfi_st	$r24, PT_R24, \docfi
		cfi_st	$r25, PT_R25, \docfi
		cfi_st	$r26, PT_R26, \docfi
		cfi_st	$r27, PT_R27, \docfi
		cfi_st	$r28, PT_R28, \docfi
		cfi_st	$r29, PT_R29, \docfi
		cfi_st	$r30, PT_R30, \docfi
		cfi_st	$r31, PT_R31, \docfi
		.endm

/*
 * get_saved_sp returns the SP for the current CPU by looking in the
 * kernelsp array for it.  If tosp is set, it stores the current sp in
 * t0 and loads the new value in sp.  If not, it clobbers t0 and
 * stores the new value in t1, leaving sp unaffected.
 */
		/* Uniprocessor variation */
		.macro	get_saved_sp docfi=0 tosp=0
		la.abs	t1, kernelsp
		.if \tosp
		move	t0, sp
		.if \docfi
		.cfi_register sp, t0
		.endif
		LONG_L	sp, t1, 0
		.else
		LONG_L	t1, t1, 0
		.endif
		.endm

		.macro	set_saved_sp stackp temp temp2
		la.abs	\temp, kernelsp
		LONG_S	\stackp, \temp, 0
		.endm

		.macro	SAVE_SOME docfi=0
		csrrd	t1, LISA_CSR_PRMD
		andi	t1, t1, 3	/* extract pplv bit */
		move	t0, sp
		beqz	t1, 8f
		//.if \docfi
		//.cfi_register sp, t0
		//.endif
		/* Called from user mode, new stack. */
		get_saved_sp docfi=\docfi tosp=1
8:
		PTR_ADDIU sp, sp, -PT_SIZE
		.if \docfi
		.cfi_def_cfa sp,0
		.endif
		cfi_st	t0, PT_R3, \docfi
		cfi_rel_offset  sp, PT_R3, \docfi
		//cfi_st	v1, PT_R3, \docfi
		/*
		 * You might think that you don't need to save $0,
		 * but the FPU emulator and gdb remote debug stub
		 * need it to operate correctly
		 */
		LONG_S	zero, sp, PT_R0
		//cfi_st	v0, PT_R2, \docfi
		cfi_st	$r4, PT_R4, \docfi
		cfi_st	$r5, PT_R5, \docfi
		csrrd	t0, LISA_CSR_PRMD
		LONG_S	t0, sp, PT_PRMD
		csrrd	t0, LISA_CSR_CRMD
		LONG_S	t0, sp, PT_CRMD
		csrrd	t0, LISA_CSR_EXCFG
		LONG_S	t0, sp, PT_EXCFG
		csrrd	t0, LISA_CSR_CU
		LONG_S  t0, sp, PT_CU
		csrrd	t0, LISA_CSR_EXST
		LONG_S  t0, sp, PT_EXST
		cfi_st	$r6, PT_R6, \docfi
		cfi_st	ra, PT_R1, \docfi
		cfi_st	$r7, PT_R7, \docfi
		cfi_st	$r8, PT_R8, \docfi
		cfi_st	$r9, PT_R9, \docfi
		cfi_st	$r10, PT_R10, \docfi
		cfi_st	$r11, PT_R11, \docfi
		csrrd	ra, LISA_CSR_EPC
		LONG_S	ra, sp, PT_EPC
		.if \docfi
		.cfi_rel_offset ra, PT_EPC
		.endif
		cfi_st	tp, PT_R2, \docfi
		cfi_st	$r21, PT_R21, \docfi
		cfi_st	fp, PT_R22, \docfi

		/* Set thread_info if we're coming from user mode */
		csrrd	t0, LISA_CSR_PRMD
		andi	t0, t0, 3	/* extract pplv bit */
		beqz	t0, 9f

		//ori	$28, sp, _THREAD_MASK
		//xori	$28, $28, _THREAD_MASK
		PTR_LI  tp, ~_THREAD_MASK
		and	tp, tp, sp
9:
		.endm

		.macro	SAVE_ALL docfi=0
		SAVE_SOME \docfi
		SAVE_TEMP \docfi
		SAVE_STATIC \docfi
		.endm

		.macro	RESTORE_TEMP docfi=0
		cfi_ld	$r12, PT_R12, \docfi
		cfi_ld	$r13, PT_R13, \docfi
		cfi_ld	$r14, PT_R14, \docfi
		cfi_ld	$r15, PT_R15, \docfi
		cfi_ld	$r16, PT_R16, \docfi
		cfi_ld	$r17, PT_R17, \docfi
		cfi_ld	$r18, PT_R18, \docfi
		cfi_ld	$r19, PT_R19, \docfi
		cfi_ld	$r20, PT_R20, \docfi
		.endm

		.macro	RESTORE_STATIC docfi=0
		cfi_ld	$r23, PT_R23, \docfi
		cfi_ld	$r24, PT_R24, \docfi
		cfi_ld	$r25, PT_R25, \docfi
		cfi_ld	$r26, PT_R26, \docfi
		cfi_ld	$r27, PT_R27, \docfi
		cfi_ld	$r28, PT_R28, \docfi
		cfi_ld	$r29, PT_R29, \docfi
		cfi_ld	$r30, PT_R30, \docfi
		cfi_ld	$r31, PT_R31, \docfi
		.endm

		.macro	RESTORE_SP docfi=0
		cfi_ld	sp, PT_R3, \docfi
		.endm

		.macro	RESTORE_SOME docfi=0
		/* loongisa clear IE and PLV*/
		li	v0, 7
		csrxchg	zero, v0, LISA_CSR_CRMD

		li	v0, ~0x7
		LONG_L	v1, sp, PT_CRMD
		csrxchg	v1, v0, LISA_CSR_CRMD

		LONG_L	v1, sp, PT_PRMD
		csrwr	v1, LISA_CSR_PRMD
		LONG_L	v1, sp, PT_EPC
		csrwr	v1, LISA_CSR_EPC
		cfi_ld	$r1, PT_R1, \docfi
		cfi_ld	$r11, PT_R11, \docfi
		cfi_ld	$r10, PT_R10, \docfi
		cfi_ld	$r9, PT_R9, \docfi
		cfi_ld	$r8, PT_R8, \docfi
		cfi_ld	$r7,  PT_R7, \docfi
		cfi_ld	$r6,  PT_R6, \docfi
		cfi_ld	$r5,  PT_R5, \docfi
		cfi_ld	$r4,  PT_R4, \docfi
		cfi_ld	tp, PT_R2, \docfi
		cfi_ld	$r21, PT_R21, \docfi
		cfi_ld	fp, PT_R22, \docfi
		.endm

		.macro	RESTORE_SP_AND_RET docfi=0
		RESTORE_SP \docfi
		ertn
		.endm

		.macro	RESTORE_ALL docfi=0
		RESTORE_TEMP \docfi
		RESTORE_STATIC \docfi
		RESTORE_SOME \docfi
		RESTORE_SP \docfi
		.endm

/*
 * Move to kernel mode and disable interrupts.
 * Set cp0 enable bit as sign that we're running on the kernel stack
 */
		.macro	CLI
		li	t0, 7
		csrxchg	zero, t0, LISA_CSR_CRMD
		.endm

/*
 * Move to kernel mode and enable interrupts.
 * Set cp0 enable bit as sign that we're running on the kernel stack
 */
		.macro	STI
		li	t0, 0x7
		li	t1, (1 << 2)
		csrxchg	t1, t0, LISA_CSR_CRMD
		.endm

/*
 * Just move to kernel mode and leave interrupts as they are.  Note
 * for the R3000 this means copying the previous enable from IEp.
 * Set cp0 enable bit as sign that we're running on the kernel stack
 */
		.macro	KMODE
		li	t0, 0x3
		li	t1, 0
		csrxchg	t1, t0, LISA_CSR_CRMD
		.endm

#endif /* _ASM_STACKFRAME_H */
