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
    extern unsigned char exception_handler[];
    extern unsigned char tlbrefill_handler[];
    extern unsigned char tlbrefill_redirector[];
    __csrwr(exception_handler, LISA_CSR_EBASE);
    __csrwr(tlbrefill_redirector, LISA_CSR_KS3);
    __csrwr((unsigned int)tlbrefill_handler & 0x1fffffff, LISA_CSR_RFBASE); // For QEMU clear high 3 bit of PA
}

void __noreturn
kern_init(void) {
    setup_exception_vector();
    
    tlb_invalidate_all();

    pic_init();                 // init interrupt controller

    
    cons_init();                // init the console
    clock_init();               // init clock interrupt

    const char *message = "(THU.CST) os is loading ...\n\n";
    kprintf(message);
    
    print_kerninfo();
#ifdef LAB2_EX1
    pmm_init();                 // init physical memory management
#else
    // For LAB1
    #if defined(_SHOW_100_TICKS) && defined(_SHOW_SERIAL_INPUT)
        kprintf("LAB1 Check - Please press your keyboard manually and see what happend.\n");
        intr_enable();
        while(1) asm volatile ("\tidle 0\n");
    #else
        kprintf("You should manually enable _SHOW_100_TICKS and _SHOW_SERIAL_INPUT in Makefile to do LAB1 Check.\n");
        intr_enable();
        while(1) asm volatile ("\tidle 0\n");
    #endif
#endif
#ifdef LAB3_EX1
    vmm_init();                 // init virtual memory management
    sched_init();
    proc_init();                // init process table
#else
    // For LAB2
    kprintf("LAB2 Check Pass!\n");
    intr_enable();
    while(1) asm volatile ("\tidle 0\n");
#endif

#ifdef LAB4_EX1
    check_initrd();
    ide_init();
    fs_init();
#endif
    intr_enable();              // enable irq interrupt
    cpu_idle();
}