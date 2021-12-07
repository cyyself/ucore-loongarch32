#include <loongarch_tlb.h>
#include <memlayout.h>

// invalidate both TLB 
// (clean and flush, meaning we write the data back)
void
tlb_invalidate(pde_t *pgdir, uintptr_t la) {
  tlb_invalidate_all();
}

void tlb_invalidate_all(){
    int i;
    for(i=0;i<128;i++)
      write_one_tlb(i, 0x80000000+(i<<20), 0, 0);
}

uint32_t pte2tlblow(pte_t pte)
{
#ifdef LAB3_EX1
/*
  LAB3 EXERCISE1: YOUR CODE
  
  1. Read LoongArch32 document to know the meaning of each bit in TLBELO.
  2. Find PPN from pte and fill the TLBELO[31:8]
  3. Call 'ptep_present' to check the pte exist
  4. Call 'ptep_u_read' and 'ptep_u_write' to set PLV3 bit and D bit respectively.
  5. return TLB EntryLo value.
*/
#endif
}

void tlb_refill(uint32_t badaddr, pte_t *pte)
{
#ifdef LAB3_EX1
/*
  LAB3 EXERCISE1: YOUR CODE

  1. Check if pte is null pointer
  2. Check if badaddr[12] is 1, if is, we should set pte=pte-1
  3. get TLBELO0 and TLBELO1 from 'pte2tlblow' function.
  3. Call tlb_replace_random. Fill TLBEHI will badaddr and clear lowest 13 bit.

*/
#endif
}

