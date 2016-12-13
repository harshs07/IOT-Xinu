#include <xinu.h>

devcall adcclose(struct dentry *devptr)
{
  kprintf("\nadcclose: start");
  struct adc_csr *adc_csrptr = (struct adc_csr *)devptr->dvcsr;

  //enable the ADC.
  kprintf("\nadcclose: adc enabled.");
  adc_csrptr->ctrl &= ~ADC_ENABLE;

  // power down the temperature sensor.
  GPIO_low(2, GPIO_PIN_10);

  kprintf("\nadcclose: end\n");
  return OK;
}
