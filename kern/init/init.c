#include <defs.h>
#include <loongarch.h>


#define UART_ADDR 0x9fafff10
#define UART_RX		0	/* In:  Receive buffer */
#define UART_TX		0	/* Out: Transmit buffer */
#define UART_LSR	5	/* In:  Line Status Register */
#define UART_LSR_THRE		0x20 /* Transmit-hold-register empty */
#define PORT(base, offset) (uint8_t *)(base + offset)

unsigned int serial_in(unsigned char *base, int offset)
{
	return readb(PORT(base, offset));
}

void serial_out(unsigned char *base, int offset, int value)
{
	writeb(value, PORT(base, offset));
}

void prom_putchar(char c)
{
	int timeout;
	unsigned char *uart_base = (unsigned char *)0x9fe001e0;
	timeout = 1024;
    
	while (((serial_in(uart_base, UART_LSR) & UART_LSR_THRE) == 0) &&
			(timeout-- > 0))
		;
    
	serial_out(uart_base, UART_TX, c);
}

void uart_prints(char *s) {
    while (*s) {
        prom_putchar(*s);
        s ++;
    }
}

void cpu_idle() {
    while(1);
}

void __noreturn
kern_init(void) {
    //setup_exception_vector();
    /* 
    // TODO:
    tlb_invalidate_all();

    pic_init();                 // init interrupt controller
    cons_init();                // init the console
    clock_init();               // init clock interrupt

    check_initrd();
    */
    const char *message = "(THU.CST) os is loading ...\n\n";
    uart_prints(message);
    //kprintf(message);

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