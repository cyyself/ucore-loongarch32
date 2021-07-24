#include <defs.h>
#include <asm/loongisa_csr.h>

static bool did_init = 0;


void
pic_enable(unsigned int irq) {
    //assert(irq < 13);
    uint32_t sr = read_csr_excfg();
    sr |= 1<<irq;
    write_csr_excfg(sr);
}

void pic_disable(unsigned int irq){
    //assert(irq < 13);
    uint32_t sr = read_csr_excfg();
    sr &= ~(1<<irq);
    write_csr_excfg(sr);
}

void
pic_init(void) {
    clear_csr_excfg(ECFG0_IM);
    did_init = 1;
}

