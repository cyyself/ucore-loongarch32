#include <loongarch.h>
#include <asm/loongisa_csr.h>
#include <trap.h>
#include <stdio.h>
#include <picirq.h>
//#include <sched.h>

volatile size_t ticks;


#define HZ 100

int clock_int_handler(void * data)
{
  ticks++;
  if (ticks % 100 == 0) kprintf("100 ticks\n");
//  if( (ticks & 0x1F) == 0)
//    cons_putc('A');
  write_csr_tmintclear(CSR_TMINTCLR_TI);
  //run_timer_list();
  //reload_timer(); 
  //no need to reload by software on loongarch
  return 0;
}

void
clock_init(void) {
  unsigned long timer_config;
  unsigned long period = 200000000;
	period = period / HZ;
	timer_config = period & LISA_CSR_TMCFG_TIMEVAL;
	timer_config |= (LISA_CSR_TMCFG_PERIOD | LISA_CSR_TMCFG_EN);
  __lcsr_csrwr(timer_config, LISA_CSR_TMCFG);
  pic_enable(TIMER0_IRQ);
  kprintf("++setup timer interrupts\n");
}

