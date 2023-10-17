#include <defs.h>
#include <loongarch.h>
#include <stdio.h>
#include <string.h>
#include <picirq.h>
#include <intr.h>

/* stupid I/O delay routine necessitated by historical PC design flaws */
static void
delay(void) {
}

/***** Serial I/O code *****/

#define COM_RX          0       // In:  Receive buffer (DLAB=0)
#define COM_TX          0       // Out: Transmit buffer (DLAB=0)
#define COM_DLL         0       // Out: Divisor Latch Low (DLAB=1)
#define COM_DLM         1       // Out: Divisor Latch High (DLAB=1)
#define COM_IER         1       // Out: Interrupt Enable Register
#define COM_IER_RDI     0x01    // Enable receiver data interrupt
#define COM_IIR         2       // In:  Interrupt ID Register
#define COM_FCR         2       // Out: FIFO Control Register
#define COM_LCR         3       // Out: Line Control Register
#define COM_LCR_DLAB    0x80    // Divisor latch access bit
#define COM_LCR_WLEN8   0x03    // Wordlength: 8 bits
#define COM_MCR         4       // Out: Modem Control Register
#define COM_MCR_RTS     0x02    // RTS complement
#define COM_MCR_DTR     0x01    // DTR complement
#define COM_MCR_OUT2    0x08    // Out2 complement
#define COM_LSR         5       // In:  Line Status Register
#define COM_LSR_DATA    0x01    // Data available
#define COM_LSR_TXRDY   0x20    // Transmit buffer avail
#define COM_LSR_TSRE    0x40    // Transmitter off


static bool serial_exists = 0;

static void
serial_init(void) {
    volatile unsigned char *uart = (unsigned char*)COM1;
    if(serial_exists)
      return ;
    serial_exists = 1;
    // Turn off the FIFO
    outb(COM1 + COM_FCR, 0);
    // Set speed; requires DLAB latch
    outb(COM1 + COM_LCR, COM_LCR_DLAB);
    // outb(COM1 + COM_DLL, (uint8_t) (COM1_BAUD_DDL));
    // don't change the baud rate set by pmon
    outb(COM1 + COM_DLM, 0);

    // 8 data bits, 1 stop bit, parity off; turn off DLAB latch
    outb(COM1 + COM_LCR, COM_LCR_WLEN8 & ~COM_LCR_DLAB);

    // No modem controls
    outb(COM1 + COM_MCR, 0);
    // Enable rcv interrupts
    outb(COM1 + COM_IER, COM_IER_RDI);

    pic_enable(COM1_IRQ);
}


static void
serial_putc_sub(int c) {
    while (!(inb(COM1 + COM_LSR) & COM_LSR_TXRDY)) {
    }
    outb(COM1 + COM_TX, c);
}

/* serial_putc - print character to serial port */
static void
serial_putc(int c) {
    if (c == '\n') {
        serial_putc_sub('\r');
        serial_putc_sub('\n');
    }else {
        serial_putc_sub(c);
    }
}

/* serial_proc_data - get data from serial port */
static int
serial_proc_data(void) {
    int c;
    if (!(inb(COM1 + COM_LSR) & COM_LSR_DATA)) {
        return -1;
    }
    c = inb(COM1 + COM_RX);
    return c;
}

#define CONSBUFSIZE 512

/* *
 * Here we manage the console input buffer, where we stash characters
 * received from the keyboard or serial port whenever the corresponding
 * interrupt occurs.
 * */



static struct {
    uint8_t buf[CONSBUFSIZE];
    uint32_t rpos;
    uint32_t wpos;
} cons;

/* *
 * cons_intr - called by device interrupt routines to feed input
 * characters into the circular console input buffer.
 * */
static void
cons_intr(int (*proc)(void)) {
    int c;
    while ((c = (*proc)()) != -1) {
        if (c != 0) {
            cons.buf[cons.wpos ++] = c;
            if (cons.wpos == CONSBUFSIZE) {
                cons.wpos = 0;
            }
        }
    }
}


/* cons_init - initializes the console devices */
void
cons_init(void) {
    serial_init();
    //cons.rpos = cons.wpos = 0;
    if (!serial_exists) {
        kprintf("serial port does not exist!!\n");
    }
}


/* serial_intr - try to feed input characters from serial port */
void
serial_intr(void) {
    if (serial_exists) {
        cons_intr(serial_proc_data);
    }
}


/* cons_putc - print a single character @c to console devices */
void
cons_putc(int c) {
    long intr_flag;
    local_intr_save(intr_flag);
    {
        serial_putc(c);
    }
    local_intr_restore(intr_flag);
}




/* *
 * cons_getc - return the next input character from console,
 * or 0 if none waiting.
 * */
int
cons_getc(void) {
    int c = 0;
    long intr_flag;
    local_intr_save(intr_flag);
    {
        // poll for any pending input characters,
        // so that this function works even when interrupts are disabled
        // (e.g., when called from the kernel monitor).
        serial_intr();

        // grab the next character from the input buffer.
        if (cons.rpos != cons.wpos) {
            c = cons.buf[cons.rpos ++];
            if (cons.rpos == CONSBUFSIZE) {
                cons.rpos = 0;
            }
        }
    }
    local_intr_restore(intr_flag);
    return c;
}

void serial_int_handler(void *opaque)
{

    /*
    此处串口处理函数的执行逻辑：（此处仅描述console.c文件范围内的执行逻辑）
    首先当ucore启动后，...,然后会根据serial_init()函数完成COM1的初始化（包括波特率、数据位、校验位等的串口基本配置），同时使其能进入中断
    1.当有外设进行输入时，会触发COM1串口的中断处理函数
    2.读取COM_IIR位是否为0x01,确定是COM1的中断状态是否已经就绪
    3.然后从Console_buffer里面获取串口输入的字符(
        此处包括以下几个操作：
        1.首先在serial_proc_data函数中根据LSR和LSR_Data的有效性判断RX寄存器是否有有效的数据，如果有，读出来
        2.然后在cons_intr函数中将读出来的串口RX中的数据存到Console_buffer里面
        3.在cons_getc函数中把buffer里面的数据读出来
        期间利用local_intr_save(intr_flag)关闭中断，然后完成处理后利用local_intr_restore(intr_flag)使中断重新开启
        )
    4.然后利用打印输出的API--kprintf，输出我们输入的字符，完成实验
    5.
    */
    unsigned char id = inb(COM1+COM_IIR);
    if(id & 0x01)
        return ;

    /*此处两种方式读数据都可行，前者是直接从RX寄存器中读的，后者是从Console_buffer里面读出来的*/
    //int c = serial_proc_data();
    int c = cons_getc();


#if defined(LAB1_EX3) && defined(_SHOW_SERIAL_INPUT)

    // LAB1 EXERCISE3: YOUR CODE
    /*利用打印输出的api,输出相应的字符*/
    kprintf("press key is: %c\n",c);


#endif
#ifdef LAB4_EX2
    extern void dev_stdin_write(char c);
    dev_stdin_write(c);
#endif
}

