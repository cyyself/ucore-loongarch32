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