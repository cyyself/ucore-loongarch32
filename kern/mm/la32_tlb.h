#ifndef __KERN_LA_TLB_H__
#define __KERN_LA_TLB_H__

#include <loongarch_tlb.h>
#include <memlayout.h>

void tlb_invalidate(pde_t *pgdir, uintptr_t la);
void tlb_invalidate_all();
uint32_t pte2tlblow(pte_t pte);
void tlb_refill(uint32_t badaddr, pte_t *pte);

#endif /* !__KERN_LA_TLB_H__ */