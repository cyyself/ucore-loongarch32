#include <trap.h>
#include <stdio.h>
#include <string.h>
#include <loongarch.h>
#include <asm/loongisa_csr.h>
#include <clock.h>
#include <console.h>
#include <assert.h>

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
    PRINT_HEX(" Status\t: ", tf->tf_estat);
    PRINT_HEX(" PRMD\t: ", tf->tf_prmd);
    PRINT_HEX(" EPC\t: ", tf->tf_era);
    //if (!trap_in_kernel(tf)) {
    //  kprintf("Trap in usermode: ");
    //}else{
      kprintf("Trap in kernel: ");
    //}
    //kprintf(trapname(GET_CAUSE_EXCODE(tf->tf_cause))); // TODO
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

static void
trap_dispatch(struct trapframe *tf) {
  int code = GET_CAUSE_EXCODE(tf->tf_estat);
  switch(code){
    case EX_IRQ:
      interrupt_handler(tf);
      break;
    case EX_RI:
      print_trapframe(tf);
      if(trap_in_kernel(tf)) {
        panic("hey man! Do NOT use that insn!");
      }
      //do_exit(-E_KILLED); TODO: 进程管理
      break;
    case EX_IPE:
      print_trapframe(tf);
      if(trap_in_kernel(tf)) {
        panic("CpU exception should not occur in kernel mode!");
      }
      //do_exit(-E_KILLED); TODO: 进程管理
    case EX_SYS:
      tf->tf_era += 4;
      //syscall(); TODO: syscall
    case EX_ADE:
      if(trap_in_kernel(tf)){
        print_trapframe(tf);
        panic("Alignment Error");
      }else{
        print_trapframe(tf);
        //do_exit(-E_KILLED);  TODO: 进程管理
      }
      break;
    default:
      print_trapframe(tf);
      panic("Unhandled Exception");
  }
}

void
loongarch_trap(struct trapframe *tf) {
  struct trapframe *tf2 = tf;
  // dispatch based on what type of trap occurred
  // used for previous projects
  //if (current == NULL) {
    trap_dispatch(tf);
  //}
  /*
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
  */
}