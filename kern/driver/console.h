#ifndef __KERN_DRIVER_CONSOLE_H__
#define __KERN_DRIVER_CONSOLE_H__

void cons_init(void);
void cons_putc(int c);
int cons_getc(void);
void serial_intr(void);
void serial_int_handler(void *opaque);
#endif /* !__KERN_DRIVER_CONSOLE_H__ */

