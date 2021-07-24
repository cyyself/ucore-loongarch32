#include <intr.h>

/* intr_enable - enable irq interrupt */
void
intr_enable(void) {
	__lcsr_csrxchg(LISA_CSR_CRMD_IE, LISA_CSR_CRMD_IE, LISA_CSR_CRMD);
}

/* intr_disable - disable irq interrupt */
void
intr_disable(void) {
	__lcsr_csrxchg(0, LISA_CSR_CRMD_IE, LISA_CSR_CRMD);
}

