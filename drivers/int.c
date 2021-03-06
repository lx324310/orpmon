/* This file is part of test microkernel for OpenRISC 1000. */
/* (C) 2001 Simon Srot, srot@opencores.org */

#include "common.h"
#include "support.h"
#include "spr-defs.h"
#include "int.h"

#ifdef OR1K

/* Interrupt handlers table */
struct ihnd int_handlers[MAX_INT_HANDLERS];

/* Initialize routine */
int int_init()
{
  int i;

  for(i = 0; i < MAX_INT_HANDLERS; i++) {
    int_handlers[i].handler = 0;
  }
  
  return 0;
}

/* Add interrupt handler */ 
int int_add(unsigned long vect, void (* handler)(void))
{
  if(vect >= MAX_INT_HANDLERS) return -1;

  int_handlers[vect].handler = handler;
  debug ("int_add %i: %08lx\n", vect, int_handlers[vect].handler);

  mtspr(SPR_PICMR, mfspr(SPR_PICMR) | (0x00000001L << vect));
  return 0;
}

/* Disable interrupt */ 
int int_disable(unsigned long vect)
{
  if(vect >= MAX_INT_HANDLERS) return -1;

  mtspr(SPR_PICMR, mfspr(SPR_PICMR) & ~(0x00000001L << vect));
  
  return 0;
}

/* Enable interrupt */ 
int int_enable(unsigned long vect)
{
  if(vect >= MAX_INT_HANDLERS) return -1;

  mtspr(SPR_PICMR, mfspr(SPR_PICMR) | (0x00000001L << vect));
  
  return 0;
}

/* Main interrupt handler */
void int_main(void)
{
  unsigned long picsr = mfspr(SPR_PICSR);
  unsigned long i = 0;

  mtspr(SPR_PICSR, 0);
  //printf ("int :%08lx\n", picsr);

  while(i < 32) {
    if((picsr & (0x01L << i)) && (int_handlers[i].handler != 0)) {
      (*int_handlers[i].handler)(); 
      mtspr(SPR_PICSR, mfspr(SPR_PICSR) & ~(0x00000001L << i));
    }
    i++;
  }
}


void
int_error(int vect, unsigned long epcr)
{
  printf("\n\nERROR - ");
  switch(vect)
    {
    case 2:
      printf("BUS ERROR (0x200)\n");
      break;
    case 6:
      printf("ALIGN ERROR (0x600)\n");
      break;
    case 7:
      printf("ILLEGAL INSN ERROR (0x700)\n");
      break;
    default:
      printf("UNKNOWN ERROR (%d)\n",vect);
      break;
    }

  printf("EPCR: 0x%.8x\n",epcr);

  printf("\n");
  
  // TODO - print some more diagnostics here
  
  printf("Attemping to reset...\n\n");
  
  asm volatile("l.ori r3, r0, 0x100");
  asm volatile("l.jr r3");
  asm volatile("l.nop");

  // Stay here forever. 
  //while(1);

}
  
#endif
