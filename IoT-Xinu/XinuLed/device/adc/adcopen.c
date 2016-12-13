#include <xinu.h>


// add mode information to the function call.
devcall adcopen(struct dentry *devptr, char *mode)
{
  kprintf("\nadcopen: start");
  //uint32 tempMode = 3;
  uint32 tempMode = *((uint32 *)mode);

  // initializing ADC clock.
  kprintf("\nadcopen: after mode");
  volatile uint32 *adc_tsc_clkctrl = (uint32 *)CM_WKUP_ADC_TSC_CLKCTRL;
  kprintf("\nadcopen:clock register value is %08x", *adc_tsc_clkctrl);

  *adc_tsc_clkctrl |= CM_WKUP_MODULEMODE_ENABLE;
  kprintf("\nadcopen:clock register after enabling is %08x", *adc_tsc_clkctrl);

  while(!(*adc_tsc_clkctrl & CM_WKUP_MODULEMODE_ENABLE))
  //while(!(*adc_tsc_clkctrl & CM_WKUP_IDLEST_DISABLED))
    kprintf("\nadcopen: Waiting for clock to activate");

  struct adc_csr *adc_csrptr = (struct adc_csr *)devptr->dvcsr;
  kprintf("\nadcopen: Value of %10x", devptr->dvcsr);

  // configure the ADC control register.
  kprintf("\nadcopen: address of adc control %10x", &(adc_csrptr->ctrl));
  // set the ADC mode to non-touch screen mode.
  adc_csrptr->ctrl &= ADC_SETUP;
  kprintf("\nadcopen: value of adc control after mode %10x", adc_csrptr->ctrl);
  adc_csrptr->ctrl |= ADC_STEPCONFIG_WRITE_PROTECT_OFF;
  kprintf("\nadcopen: value of adc control after write protect %10x", adc_csrptr->ctrl);

  // configure step enable register.
  // 1. Reset the register. 2. Enable only step 1.
  adc_csrptr->step_enable &= ADC_RESET;
  adc_csrptr->step_enable |= ADC_STEP_ENABLE_1;

  //step configuration.
  // value of step 1 config register.
  kprintf("\nadcopen: value of adc step config %10x", adc_csrptr->step[0].step_config);
  // reseting step config 1. Reset changes step to all required values.
  adc_csrptr->step[0].step_config &= ADC_RESET;
  kprintf("\nadcopen: value of adc step config after reset %10x", adc_csrptr->step[0].step_config);

  // add code here to enable averaging modes.
  kprintf("\nadcopen: Mode value received is %d", tempMode);
  #include<tempMode-ddl-out>
  /*switch(tempMode)
  {
    case 1:
    {
      adc_csrptr->step[0].step_config |= STEP_AVG0;
      break;
    }
    case 2:
    {
      adc_csrptr->step[0].step_config |= STEP_AVG8;
      break;
    }
    case 3:
    {
      adc_csrptr->step[0].step_config |= STEP_AVG16;
      break;
    }
  }*/
  // clearing interrupt status.
  adc_csrptr->irq_status &= ADC_RESET;
  // enabling FIFO0 threshold interrupt.
  adc_csrptr->irq_enable_set &= ADC_RESET;
  adc_csrptr->irq_enable_set |= THRESHOLD_INTERRUPT;

  //enable the GPIO high pin to power on the temp controller.
  GPIO_high(2, GPIO_PIN_10);

  //enable the ADC.
  kprintf("\nadcopen: adc enabled.");
  adc_csrptr->ctrl |= ADC_ENABLE;

  kprintf("\nadcopen: end\n");
  return OK;
}
