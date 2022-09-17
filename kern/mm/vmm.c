#include <loongarch.h>
#include <vmm.h>
#include <sync.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <error.h>
#include <kmalloc.h>
#include <pmm.h>

/* 
   vmm design include two parts: mm_struct (mm) & vma_struct (vma)
   mm is the memory manager for the set of continuous virtual memory  
   area which have the same PDT. vma is a continuous virtual memory area.
   There a linear link list for vma & a redblack link list for vma in mm.
   ---------------
   mm related functions:
   golbal functions
   struct mm_struct * mm_create(void)
   void mm_destroy(struct mm_struct *mm)
   int do_pgfault(struct mm_struct *mm, uint32_t error_code, uintptr_t addr)
   --------------
   vma related functions:
   global functions
   struct vma_struct * vma_create (uintptr_t vm_start, uintptr_t vm_end,...)
   void insert_vma_struct(struct mm_struct *mm, struct vma_struct *vma)
   struct vma_struct * find_vma(struct mm_struct *mm, uintptr_t addr)
   local functions
   inline void check_vma_overlap(struct vma_struct *prev, struct vma_struct *next)
   ---------------
   check correctness functions
   void check_vmm(void);
   void check_vma_struct(void);
   void check_pgfault(void);
   */

static void check_vmm(void);
static void check_vma_struct(void);
static void check_pgfault(void);

// mm_create -  alloc a mm_struct & initialize it.
struct mm_struct *
mm_create(void) {
  struct mm_struct *mm = kmalloc(sizeof(struct mm_struct));

  if (mm != NULL) {
    list_init(&(mm->mmap_list));
    mm->mmap_cache = NULL;
    mm->pgdir = NULL;
    mm->map_count = 0;

    set_mm_count(mm, 0);
    sem_init(&(mm->mm_sem), 1);
  }	
  return mm;
}

// vma_create - alloc a vma_struct & initialize it. (addr range: vm_start~vm_end)
struct vma_struct *
vma_create(uintptr_t vm_start, uintptr_t vm_end, uint32_t vm_flags) {
  struct vma_struct *vma = kmalloc(sizeof(struct vma_struct));

  if (vma != NULL) {
    vma->vm_start = vm_start;
    vma->vm_end = vm_end;
    vma->vm_flags = vm_flags;
  }
  return vma;
}


// find_vma - find a vma  (vma->vm_start <= addr <= vma_vm_end)
struct vma_struct *
find_vma(struct mm_struct *mm, uintptr_t addr) {
  struct vma_struct *vma = NULL;
  if (mm != NULL) {
    vma = mm->mmap_cache;
    if (!(vma != NULL && vma->vm_start <= addr && vma->vm_end > addr)) {
      bool found = 0;
      list_entry_t *list = &(mm->mmap_list), *le = list;
      while ((le = list_next(le)) != list) {
        vma = le2vma(le, list_link);
        if (addr < vma->vm_end) {
          found = 1;
          break;
        }
      }
      if (!found) {
        vma = NULL;
      }
    }
    if (vma != NULL) {
      mm->mmap_cache = vma;
    }
  }
  return vma;
}


// check_vma_overlap - check if vma1 overlaps vma2 ?
static inline void
check_vma_overlap(struct vma_struct *prev, struct vma_struct *next) {
  assert(prev->vm_start < prev->vm_end);
  assert(prev->vm_end <= next->vm_start);
  assert(next->vm_start < next->vm_end);
}


// insert_vma_struct -insert vma in mm's list link
void
insert_vma_struct(struct mm_struct *mm, struct vma_struct *vma) {
  assert(vma->vm_start < vma->vm_end);
  list_entry_t *list = &(mm->mmap_list);
  list_entry_t *le_prev = list, *le_next;

  list_entry_t *le = list;
  while ((le = list_next(le)) != list) {
    struct vma_struct *mmap_prev = le2vma(le, list_link);
    if (mmap_prev->vm_start > vma->vm_start) {
      break;
    }
    le_prev = le;
  }

  le_next = list_next(le_prev);

  /* check overlap */
  if (le_prev != list) {
    check_vma_overlap(le2vma(le_prev, list_link), vma);
  }
  if (le_next != list) {
    check_vma_overlap(vma, le2vma(le_next, list_link));
  }

  vma->vm_mm = mm;
  list_add_after(le_prev, &(vma->list_link));

  mm->map_count ++;
}

// mm_destroy - free mm and mm internal fields
void
mm_destroy(struct mm_struct *mm) {
  assert(mm_count(mm) == 0);

  list_entry_t *list = &(mm->mmap_list), *le;
  while ((le = list_next(list)) != list) {
    list_del(le);
    kfree(le2vma(le, list_link));  //kfree vma		
  }
  kfree(mm); //kfree mm
  mm=NULL;
}


int
mm_map(struct mm_struct *mm, uintptr_t addr, size_t len, uint32_t vm_flags,
    struct vma_struct **vma_store) {
  uintptr_t start = ROUNDDOWN_2N(addr, PGSHIFT), end = ROUNDUP_2N(addr + len, PGSHIFT);
  if (!USER_ACCESS(start, end)) {
    return -E_INVAL;
  }

  assert(mm != NULL);

  int ret = -E_INVAL;

  struct vma_struct *vma;
  if ((vma = find_vma(mm, start)) != NULL && end > vma->vm_start) {
    goto out;
  }
  ret = -E_NO_MEM;

  if ((vma = vma_create(start, end, vm_flags)) == NULL) {
    goto out;
  }
  insert_vma_struct(mm, vma);
  if (vma_store != NULL) {
    *vma_store = vma;
  }
  ret = 0;

out:
  return ret;
}

int
dup_mmap(struct mm_struct *to, struct mm_struct *from) {
  assert(to != NULL && from != NULL);
  list_entry_t *list = &(from->mmap_list), *le = list;
  while ((le = list_prev(le)) != list) {
    struct vma_struct *vma, *nvma;
    vma = le2vma(le, list_link);
    nvma = vma_create(vma->vm_start, vma->vm_end, vma->vm_flags);
    if (nvma == NULL) {
      return -E_NO_MEM;
    }

    insert_vma_struct(to, nvma);

    bool share = 0;
    if (copy_range(to->pgdir, from->pgdir, vma->vm_start, vma->vm_end, share) != 0) {
      return -E_NO_MEM;
    }
  }
  return 0;
}

void
exit_mmap(struct mm_struct *mm) {
  assert(mm != NULL && mm_count(mm) == 0);
  pde_t *pgdir = mm->pgdir;
  list_entry_t *list = &(mm->mmap_list), *le = list;
  while ((le = list_next(le)) != list) {
    struct vma_struct *vma = le2vma(le, list_link);
    unmap_range(pgdir, vma->vm_start, vma->vm_end);
  }
  while ((le = list_next(le)) != list) {
    struct vma_struct *vma = le2vma(le, list_link);
    exit_range(pgdir, vma->vm_start, vma->vm_end);
  }
}

bool
copy_from_user(struct mm_struct *mm, void *dst, const void *src, size_t len, bool writable) {
  if (!user_mem_check(mm, (uintptr_t)src, len, writable)) {
    return 0;
  }
  memcpy(dst, src, len);
  return 1;
}

bool
copy_to_user(struct mm_struct *mm, void *dst, const void *src, size_t len) {
  if (!user_mem_check(mm, (uintptr_t)dst, len, 1)) {
    return 0;
  }
  memcpy(dst, src, len);
  return 1;
}


// vmm_init - initialize virtual memory management
//          - now just call check_vmm to check correctness of vmm
void
vmm_init(void) {
  check_vmm();
}

// check_vmm - check correctness of vmm
static void
check_vmm(void) {
  size_t nr_free_pages_store = nr_free_pages();

  check_vma_struct();
  check_pgfault();

  assert(nr_free_pages_store == nr_free_pages());

  kprintf("check_vmm() succeeded.\n");
}

static void
check_vma_struct(void) {
  size_t nr_free_pages_store = nr_free_pages();

  struct mm_struct *mm = mm_create();
  assert(mm != NULL);

  int step1 = 10, step2 = step1 * 10;

  int i;
  for (i = step1; i >= 0; i --) {
    struct vma_struct *vma = vma_create(i * 5, i * 5 + 2, 0);
    assert(vma != NULL);
    insert_vma_struct(mm, vma);
  }

  for (i = step1 + 1; i <= step2; i ++) {
    struct vma_struct *vma = vma_create(i * 5, i * 5 + 2, 0);
    assert(vma != NULL);
    insert_vma_struct(mm, vma);
  }

  list_entry_t *le = list_next(&(mm->mmap_list));

  for (i = 0; i <= step2; i ++) {
    assert(le != &(mm->mmap_list));
    struct vma_struct *mmap = le2vma(le, list_link);
    assert(mmap->vm_start == i * 5 && mmap->vm_end == i * 5 + 2);
    le = list_next(le);
  }

  for (i = 0; i < 5 * step2 + 2; i ++) {
    struct vma_struct *vma = find_vma(mm, i);
    assert(vma != NULL);
    int j = __divu5(i);
    if (i >= 5 * j + 2) {
      j ++;
    }
    assert(vma->vm_start == j * 5 && vma->vm_end == j * 5 + 2);
  }

  mm_destroy(mm);

  assert(nr_free_pages_store == nr_free_pages());

  kprintf("check_vma_struct() succeeded!\n");
}

struct mm_struct *check_mm_struct;

// check_pgfault - check correctness of pgfault handler
static void
check_pgfault(void) {
  size_t nr_free_pages_store = nr_free_pages();

  check_mm_struct = mm_create();
  assert(check_mm_struct != NULL);

  struct mm_struct *mm = check_mm_struct;
  pde_t *pgdir = mm->pgdir = boot_pgdir;
  assert(pgdir[0] == 0);

  struct vma_struct *vma = vma_create(0, PTSIZE, VM_WRITE);
  assert(vma != NULL);

  insert_vma_struct(mm, vma);

  uintptr_t addr = 0x100;
  assert(find_vma(mm, addr) == vma);

  int i, sum = 0;
  for (i = 0; i < 100; i ++) {
    *(char *)(addr + i) = i;
    sum += i;
  }
  for (i = 0; i < 100; i ++) {
    sum -= *(char *)(addr + i);
  }
  assert(sum == 0);

  page_remove(pgdir, ROUNDDOWN_2N(addr, PGSHIFT));
  free_page(pa2page(pgdir[0]));
  pgdir[0] = 0;

  mm->pgdir = NULL;
  mm_destroy(mm);
  check_mm_struct = NULL;

  assert(nr_free_pages_store == nr_free_pages());

  kprintf("check_pgfault() succeeded!\n");
}



//page fault number
volatile unsigned int pgfault_num=0;
// do_pgfault - interrupt handler to process the page fault execption
int
do_pgfault(struct mm_struct *mm, uint32_t error_code, uintptr_t addr) {
  int ret = -E_INVAL;
  struct vma_struct *vma = find_vma(mm, addr);
  //kprintf("## %08x %08x\n", error_code, addr);

  pgfault_num++;
  if (vma == NULL || vma->vm_start > addr) {
    kprintf("not valid addr %x, and  can not find it in vma\n", addr);
    goto failed;
  }

  switch (error_code & 3) {
    default:
      /* default is 3: write, present */
    case 2: /* write, not present */
      if (!(vma->vm_flags & VM_WRITE)) {
        kprintf("write, not present in do_pgfault failed\n");
        goto failed;
      }
      break;
    case 1: /* read, present */
      kprintf("read, present in do_pgfault failed\n");
      goto failed;
    case 0: /* read, not present */
      if (!(vma->vm_flags & (VM_READ | VM_EXEC))) {
        kprintf("read, not present in do_pgfault failed\n");
        goto failed;
      }
  }

  //kprintf("## check OK\n");

  uint32_t perm = PTE_U;
  if (vma->vm_flags & VM_WRITE) {
    perm |= PTE_W;
  }
  addr = ROUNDDOWN_2N(addr, PGSHIFT);

  ret = -E_NO_MEM;

  pte_t *ptep=NULL;
  if ((ptep = get_pte(mm->pgdir, addr, 1)) == NULL) {
    goto failed;
  }

  if (*ptep == 0) { // if the phy addr isn't exist, then alloc a page & map the phy addr with logical addr
    if (pgdir_alloc_page(mm->pgdir, addr, perm) == NULL) {
      goto failed;
    }
  }
  else { // if this pte is a swap entry, then load data from disk to a page with phy addr, 
      kprintf("no swap in ucore-la32 but ptep is %x, failed\n",*ptep);
  }
  ret = 0;
failed:
  return ret;
}


bool
user_mem_check(struct mm_struct *mm, uintptr_t addr, size_t len, bool write) {
  if (mm != NULL) {
    if (!USER_ACCESS(addr, addr + len)) {
      return 0;
    }
    struct vma_struct *vma;
    uintptr_t start = addr, end = addr + len;
    while (start < end) {
      if ((vma = find_vma(mm, start)) == NULL || start < vma->vm_start) {
        return 0;
      }
      if (!(vma->vm_flags & ((write) ? VM_WRITE : VM_READ))) {
        return 0;
      }
      if (write && (vma->vm_flags & VM_STACK)) {
        if (start < vma->vm_start + PGSIZE) { //check stack start & size
          return 0;
        }
      }
      start = vma->vm_end;
    }
    return 1;
  }
  return KERN_ACCESS(addr, addr + len);
}

bool
copy_string(struct mm_struct *mm, char *dst, const char *src, size_t maxn) 
{
  size_t alen, part = ROUNDDOWN_2N((uintptr_t)src + PGSIZE, PGSHIFT) - (uintptr_t)src;
  while (1) {
    if (part > maxn) {
      part = maxn;
    }
    if (!user_mem_check(mm, (uintptr_t)src, part, 0)) {
      return 0;
    }
    if ((alen = strnlen(src, part)) < part) {
      memcpy(dst, src, alen + 1);
      return 1;
    }
    if (part == maxn) {
      return 0;
    }
    memcpy(dst, src, part);
    dst += part, src += part, maxn -= part;
    part = PGSIZE;
  }
}
