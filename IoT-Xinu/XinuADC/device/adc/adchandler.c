#include <xinu.h>

void adchandler(uint32 xnum)
{
  kprintf("\nadchandler: This function is called %d", xnum);
  volatile uint32 irq_clear = 0x01<<2;
  //volatile uint32 tempVoltage = 0;
  struct	dentry *devptr = (struct dentry *) &devtab[TMPADC];
	struct	adc_csr *adc_csrptr = (struct adc_csr *) devptr->dvcsr;

  kprintf("\nadchandler: Irq Status set to clear");
  adc_csrptr->irq_enable_clr |= irq_clear;

  // taking temperature value from FIFO0.
  kprintf("\nadchandler: reading ADC Temperature.");
  kprintf("\nadchandler: number of values in fifo0 - %d", adc_csrptr->fifoInfo[0].fifo_count);
  //int i = 0;
  // for(i=0; i < 5;i++)
  // {
  tempVoltage = (adc_csrptr->fifo_data0[1] & ADC_FIFO_MASK);
  kprintf("\nadchandler: received voltage %d", tempVoltage);
  // tempRead = ((((tempVoltage * 1.0) / 4095.0)* 1800) - 500) / 10;
  // tempRead = (uint32)(tempRead*1000);
  // kprintf("\nvoltage value received:- %d", tempVoltage);
  // tempRead = (double)tempVoltage / 4095.0;
  // kprintf("\n1.temperature value received: %d", (uint32)(tempRead*1000));
  // tempRead = tempRead * 1.8;
  // kprintf("\n2.temperature value received: %lf", tempRead);
  // tempRead = tempRead - 0.5;
  // kprintf("\n3.temperature value received: %lf", tempRead);
  // tempRead = tempRead * 100;
  // kprintf("\n4.temperature value received: %lf", tempRead);

  //}

  kprintf("\nadchandler: signalling semaphore.");
  //close(TMPADC);
  signal(semADC);

  return;
}
