#include <defs.h>
#include <loongarch.h>
#include <picirq.h>

void cpu_idle() {
    while(1);
}

void __noreturn
kern_init(void) {
    
    
    // TODO:
    //tlb_invalidate_all();

    pic_init();                 // init interrupt controller

    
    cons_init();                // init the console
    /*
    clock_init();               // init clock interrupt

    check_initrd();
    */
    const char *message = "(THU.CST) os is loading ...\n\n";
    //uart_prints(message);
    kprintf(message);

    /*
    print_kerninfo();

    pmm_init();                 // init physical memory management

    vmm_init();                 // init virtual memory management
    sched_init();
    proc_init();                // init process table

    ide_init();
    fs_init();

    intr_enable();              // enable irq interrupt
    //*(int*)(0x00124) = 0x432;
    //asm volatile("divu $1, $1, $1");
    */
    cpu_idle();
}