#include <trap.h>
#include <stdio.h>
#include <string.h>
#include <loongarch.h>
#include <asm/loongisa_csr.h>
#include <clock.h>
#include <console.h>
#include <assert.h>
#include <pmm.h>
#include <proc.h>
#include <error.h>
#include <glue_pgmap.h>
#include <loongarch_tlb.h>

#define EXCCODE_GENERIC 32
#define vec_size 512
#define PPLV 3

#define  CAUSEB_EXCCODE		16
#define  CAUSEF_EXCCODE		(_ULCAST_(63)  <<  CAUSEB_EXCCODE)
#define GET_CAUSE_EXCODE(x)   ( ((x) & CAUSEF_EXCCODE) >> CAUSEB_EXCCODE)
static void local_flush_icache_range(unsigned long start, unsigned long end)
{
	asm volatile ("\tibar 0\n"::);
}

bool
trap_in_kernel(struct trapframe *tf) {
  return !(tf->tf_prmd & PPLV);
}

void print_regs(struct pushregs *regs)
{
  int i;
  for (i = 0; i < 30; i++) {
    kprintf(" $");
    printbase10(i+2);
    kprintf("\t: ");
    printhex(regs->reg_r[i]);
    kputchar('\n');
  }
}

void
print_trapframe(struct trapframe *tf) {
    PRINT_HEX("trapframe at ", tf);
    print_regs(&tf->tf_regs);
    PRINT_HEX(" $ra\t: ", tf->tf_ra);
    PRINT_HEX(" BadVA\t: ", tf->tf_badv);
    PRINT_HEX(" Estat\t: ", tf->tf_estat);
    PRINT_HEX(" PRMD\t: ", tf->tf_prmd);
    PRINT_HEX(" EPC\t: ", tf->tf_era);
    if (!trap_in_kernel(tf)) {
      kprintf("Trap in usermode: ");
    }else{
      kprintf("Trap in kernel: ");
    }
    //kprintf(trapname(GET_CAUSE_EXCODE(tf->tf_estat)));
    kputchar('\n');
}

static void interrupt_handler(struct trapframe *tf)
{
  extern clock_int_handler(void*);
  extern serial_int_handler(void*);
  int i;
  for(i=0;i<13;i++){
    if(tf->tf_estat & (1<<i)){
      switch(i){
        case TIMER0_IRQ:
          clock_int_handler(NULL);
          break;
        case COM1_IRQ:
          serial_int_handler(NULL);
          break;
        default:
          print_trapframe(tf);
          panic("Unknown interrupt!");
      }
    }
  }
}

extern pde_t *current_pgdir;

static inline int get_error_code(int write, pte_t *pte)
{
  int r = 0;
  if(pte!=NULL && ptep_present(pte))
    r |= 0x01;
  if(write)
    r |= 0x02;
  return r;
}

static int
pgfault_handler(struct trapframe *tf, uint32_t addr, uint32_t error_code) {
  extern struct mm_struct *check_mm_struct;
  if(check_mm_struct !=NULL) { //used for test check_swap
    //print_pgfault(tf);
  }
  struct mm_struct *mm;
  if (check_mm_struct != NULL) {
    assert(current == idleproc);
    mm = check_mm_struct;
  }
  else {
    if (current == NULL) {
      print_trapframe(tf);
      //print_pgfault(tf);
      panic("unhandled page fault.\n");
    }
    mm = current->mm;
  }
  return do_pgfault(mm, error_code, addr);
}

/* use software emulated X86 pgfault */
static void handle_tlbmiss(struct trapframe* tf, int write)
{

  static int entercnt = 0;
  entercnt ++;
  //kprintf("## enter handle_tlbmiss %d times\n", entercnt);
  int in_kernel = trap_in_kernel(tf);
  uint32_t badaddr = tf->tf_badv;
  int ret = 0;
  pte_t *pte = get_pte(current_pgdir, tf->tf_badv, 0);
  if(pte==NULL || ptep_invalid(pte)){   //PTE miss, pgfault
    //tlb will not be refill in do_pgfault,
    //so a vmm pgfault will trigger 2 exception
    //permission check in tlb miss
    ret = pgfault_handler(tf, badaddr, get_error_code(write, pte));
  }else{ //tlb miss only, reload it
    /* refill two slot */
    /* check permission */
    if(in_kernel){
      tlb_refill(badaddr, pte); 
      return;
    }else{
      if (write != 2) {
        if(!ptep_u_read(pte)){
          ret = -1;
          goto exit;
        }
        if(write && !ptep_u_write(pte)){
          ret = -2;
          goto exit;
        }
      }
      tlb_refill(badaddr, pte);
      return ;
    }
  }

exit:
  if(ret){
    print_trapframe(tf);
    if(in_kernel){
      panic("unhandled pgfault");
    }else{
       do_exit(-E_KILLED); 
    }
  }
  return ;
}

static void
trap_dispatch(struct trapframe *tf) {
  int code = GET_CAUSE_EXCODE(tf->tf_estat);
  switch(code){
    case EX_IRQ:
      interrupt_handler(tf);
      break;
    case EX_TLBL:
      handle_tlbmiss(tf, 0);
      break;
    case EX_TLBS:
      handle_tlbmiss(tf, 1);
      break;
    case EX_TLBR:
      handle_tlbmiss(tf, 2);
      break;
    case EX_TLBI:
      handle_tlbmiss(tf, 0);
      break;
    case EX_RI:
      print_trapframe(tf);
      if(trap_in_kernel(tf)) {
        panic("hey man! Do NOT use that insn!");
      }
      do_exit(-E_KILLED); 
      break;
    case EX_IPE:
      print_trapframe(tf);
      if(trap_in_kernel(tf)) {
        panic("CpU exception should not occur in kernel mode!");
      }
      do_exit(-E_KILLED); 
      break;
    case EX_SYS:
      tf->tf_era += 4;
      syscall();
      break;
    case EX_ADE:
      if(trap_in_kernel(tf)){
        print_trapframe(tf);
        panic("Alignment Error");
      }else{
        print_trapframe(tf);
        do_exit(-E_KILLED);  
      }
      break;
    default:
      print_trapframe(tf);
      kprintf("Ecode = %x\n",code);
      panic("Unhandled Exception");
  }
}

void
loongarch_trap(struct trapframe *tf) {
  struct trapframe *tf2 = tf;
  // dispatch based on what type of trap occurred
  // used for previous projects
  if (current == NULL) {
    trap_dispatch(tf);
  }
  
  else {
    // keep a trapframe chain in stack
    struct trapframe *otf = current->tf;
    current->tf = tf;

    bool in_kernel = trap_in_kernel(tf);

    trap_dispatch(tf);

    current->tf = otf;
    if (!in_kernel) {
      if (current->flags & PF_EXITING) {
         do_exit(-E_KILLED); 
      }
      if (current->need_resched) {
        schedule();
      }
    }
  }
  
}