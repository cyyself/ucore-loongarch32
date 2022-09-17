#include <loongarch_tlb.h>
#include <memlayout.h>

// invalidate both TLB 
// (clean and flush, meaning we write the data back)
void
tlb_invalidate(pde_t *pgdir, uintptr_t la) {
  tlb_invalidate_all();
}

void tlb_invalidate_all(){
  asm volatile(".word 0x6498000");
}

uint32_t pte2tlblow(pte_t pte)
{
  uint32_t t = (((uint32_t)pte - KERNBASE ) >> 12)<<8;
  if(!ptep_present(&pte))
    return 0;
  t |= LOONGARCH_TLB_ENTRYL_V;
  t |= LOONGARCH_TLB_ENTRYL_G;
  t |= LOONGARCH_TLB_MAT_CO;
  if (ptep_u_read(&pte)) {
    t |= LOONGARCH_TLB_PLV3;
  }
  if(ptep_s_write(&pte))
    t |= LOONGARCH_TLB_ENTRYL_D;
  return t;
}

void tlb_refill(uint32_t badaddr, pte_t *pte)
{
  if(!pte)
    return ;
  if(badaddr & (1<<12))
    pte--;
  tlb_replace_random(badaddr & LOONGARCH_TLB_ENTRYH_VPPN_MASK, 
      pte2tlblow(*pte), pte2tlblow(*(pte+1)));
}

