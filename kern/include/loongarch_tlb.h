/*
 * =====================================================================================
 *
 *       Filename:  loongarch_tlb.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27/07/2021 3:41:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yangyu Chen, cyyself@cqu.edu.cn
 *   Organization:  Chongqing University
 *
 * =====================================================================================
 */
#ifndef _LOONGARCH_TLB_H
#define _LOONGARCH_TLB_H

#include <asm/loongisa_csr.h>
#include <glue_pgmap.h>

#define LOONGARCH_TLB_ENTRYL_V (1<<0)
#define LOONGARCH_TLB_ENTRYL_D (1<<1)
#define LOONGARCH_TLB_ENTRYL_G (1<<6)
#define LOONGARCH_TLB_ENTRYH_VPPN_MASK (~0x1FFF)

static inline void write_one_tlb(int index, unsigned int hi, unsigned int low0, unsigned int low1)
{
    write_csr_tlbidx(index | 12 << 24);
    write_csr_entryhi(hi);
    write_csr_entrylo0(low0);
    write_csr_entrylo1(low1);
    __asm__ __volatile__("tlbwr");
}

static inline void tlb_replace_random(unsigned int hi, unsigned int low0, unsigned int low1)
{
    write_csr_entryhi(hi);
    write_csr_entrylo0(low0);
    write_csr_entrylo1(low1);
    __asm__ __volatile__("tlbfill");
}



#define PTE2TLBLOW(x) (((((uint32_t)(*(x))-KERNBASE)>> 12)<<8)|LOONGARCH_TLB_ENTRYL_V|LOONGARCH_TLB_ENTRYL_D|(3<<2)|(1<<4))
// PLV=3 MAT=1

static inline uint32_t pte2tlblow(pte_t pte)
{
  uint32_t t = (((uint32_t)pte - KERNBASE ) >> 12)<<8;
  if(!ptep_present(&pte))
    return 0;
  t |= LOONGARCH_TLB_ENTRYL_V;
  /* always ignore ASID */
  t |= LOONGARCH_TLB_ENTRYL_G;
  t |= (3<<3);
  if(ptep_s_write(&pte))
    t |= LOONGARCH_TLB_ENTRYL_D;
  return t;
}

static inline void tlb_refill(uint32_t badaddr, pte_t *pte)
{
  if(!pte)
    return ;
  if(badaddr & (1<<12))
    pte--;
  tlb_replace_random(badaddr & LOONGARCH_TLB_ENTRYH_VPPN_MASK, 
      pte2tlblow(*pte), pte2tlblow(*(pte+1)));
}

#endif