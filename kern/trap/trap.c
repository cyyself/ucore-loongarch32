#include <trap.h>
#include <stdio.h>
#include <string.h>
#include <loongarch_trapframe.h>
#include <loongarch.h>
#include <asm/loongisa_csr.h>
#include <clock.h>

#define EXCCODE_GENERIC 32
#define vec_size 512

#define  CAUSEB_EXCCODE		16
#define  CAUSEF_EXCCODE		(_ULCAST_(63)  <<  CAUSEB_EXCCODE)
#define GET_CAUSE_EXCODE(x)   ( ((x) & CAUSEF_EXCCODE) >> CAUSEB_EXCCODE)
static void local_flush_icache_range(unsigned long start, unsigned long end)
{
	asm volatile ("\tibar 0\n"::);
}

/* Install CPU exception handler */
void set_handler(unsigned long offset, void *addr, unsigned long size)
{
  extern unsigned char __exception_vector[];
  memcpy((void *)(__exception_vector + offset), addr, size);
	local_flush_icache_range(__exception_vector + offset, __exception_vector + offset + size);
}

void set_exception_handler() {
  extern unsigned char __exception_vector[], ramExcHandle_general[];
  kprintf("ebase = 0x%x\n",__exception_vector);
  //set_handler(EXCCODE_GENERIC * vec_size , &ramExcHandle_general, vec_size);
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
  //extern serial_int_handler(void*);
  int i;
  for(i=0;i<13;i++){
    if(tf->tf_estat & (1<<i)){
      switch(i){
        case TIMER0_IRQ:
          clock_int_handler(NULL);
          break;
        case COM1_IRQ:
          //serial_int_handler(NULL);
          break;
        default:
          print_trapframe(tf);
          kprintf("Unhandled Exception");
          while(1); // panic
          //panic("Unknown interrupt!");
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
    default:
      print_trapframe(tf);
      kprintf("Unhandled Exception");
      while(1); // panic
      //panic("Unhandled Exception");
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