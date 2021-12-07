#include <stdio.h>
#include <ulib.h>

int
main(void) {
    cprintf("I read %08x from 0xa0010000u!\n", *(unsigned int*)0xa0010000u);
    // if you tried change the address less than the end of the elf end symbol, you will fail.
    // I think it's a security bug from QEMU by loongson not from uCore Kernel because it doesn't throw any exception.
    // The following code will cause kernel crash:
    //for (unsigned int *i=0xa0001000u;i<=0xa1000000;i++) *i = 0;
    panic("FAIL: T.T\n");
}

