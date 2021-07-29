#include <defs.h>
#include <loongarch.h>
#include <asm/loongisa_csr.h>
#include <picirq.h>
#include <trap.h>
#include <kdebug.h>
#include <pmm.h>
#include <vmm.h>
#include <proc.h>
#include <sched.h>
#include <ide.h>
#include <fs.h>


void setup_exception_vector()
{
    extern unsigned char __exception_vector[];
    __lcsr_csrwr(__exception_vector + 0x4000, LISA_CSR_EBASE);
    __lcsr_csrwr(__exception_vector + 0x4000, LISA_CSR_RFBASE);
}

void __noreturn
kern_init(void) {
    setup_exception_vector();
    
    tlb_invalidate_all();

    pic_init();                 // init interrupt controller

    
    cons_init();                // init the console
    clock_init();               // init clock interrupt

    check_initrd();

    const char *message = "(THU.CST) os is loading ...\n\n";
    kprintf(message);
    
    print_kerninfo();
    
    pmm_init();                 // init physical memory management

    vmm_init();                 // init virtual memory management
    
    sched_init();
    proc_init();                // init process table

    ide_init();
    fs_init();
    
    intr_enable();              // enable irq interrupt
    cpu_idle();
}