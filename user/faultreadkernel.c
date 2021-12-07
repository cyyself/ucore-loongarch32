#include <stdio.h>
#include <ulib.h>

int
main(void) {
    cprintf("I read %08x from 0xaf000000!\n", *(unsigned int*)0xaf000000u);
    // If you tried change the address lower than the page of the end of the kernel elf file, you will fail.
    // I think it's a security bug from QEMU by loongson not from uCore Kernel because it doesn't throw any exception.
    // The following code will cause kernel crash:
    //for (unsigned int *i=0xa0001000u;i<=0xa1000000;i++) *i = 0;
    panic("FAIL: T.T\n");
}

