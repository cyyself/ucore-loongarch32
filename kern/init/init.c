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
    // TODO:write physical address

    __lcsr_csrwr(exception_handler, LISA_CSR_EBASE);
    __lcsr_csrwr(exception_handler, LISA_CSR_RFBASE); // For QEMU clear high 3 bit of PA
    // uint32_t sr0 = __lcsr_csrrd(LISA_CSR_RFBASE);
    uint32_t sr_rfbase = __lcsr_csrrd(LISA_CSR_RFBASE)&0x1fffffff;
    // uint32_t sr_ebase = __lcsr_csrrd(LISA_CSR_EBASE)&0x1fffffff;
    // kprintf("before sr print\n");
    kprintf("%x\n",sr_rfbase);
    // kprintf("%x\n",sr_ebase);
    __lcsr_csrwr(sr_rfbase,LISA_CSR_RFBASE);
    // __lcsr_csrwr(sr_ebase,LISA_CSR_EBASE);
    // uint32_t sr1 = __lcsr_csrrd(LISA_CSR_RFBASE);
    // kprintf("%x\n",sr1);
    
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
    kprintf("LAB1 Check - Please press your keyboard manually and see what happend.\n");
    intr_enable();
    while(1);
#endif
#ifdef LAB3_EX1
    vmm_init();                 // init virtual memory management
#else
    // For LAB2
    kprintf("LAB2 Check Pass!\n");
    intr_enable();
    while(1);
#endif
#ifndef LAB4_EX1
    // For LAB3
    kprintf("LAB3 Check Pass!\n");
    intr_enable();
    while(1);
#endif
    sched_init();
    proc_init();                // init process table
#ifdef LAB8_EX2
    check_initrd();
    ide_init();
    fs_init();
#endif
    intr_enable();              // enable irq interrupt
    cpu_idle();
}