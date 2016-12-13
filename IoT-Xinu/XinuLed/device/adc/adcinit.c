#include <xinu.h>

devcall adcinit(struct dentry *devptr)
{
  uint32 channel_select = 1;
  kprintf("\nadcinit: start");

  // initializing clock for ADC.
  volatile uint32 *adc_tsc_clkctrl = (uint32 *)CM_WKUP_ADC_TSC_CLKCTRL;
  kprintf("\nadcinit:1. Generated Value is %08x", *adc_tsc_clkctrl);

  *adc_tsc_clkctrl |= CM_WKUP_MODULEMODE_ENABLE;
  kprintf("\nadcinit:2. Generated Value is %08x", *adc_tsc_clkctrl);

  while(!(*adc_tsc_clkctrl & CM_WKUP_MODULEMODE_ENABLE))
  //while(!(*adc_tsc_clkctrl & CM_WKUP_IDLEST_DISABLED))
    kprintf("\nadcinit: Waiting for clock to activate");

  struct adc_csr *adc_csrptr = (struct adc_csr *)devptr->dvcsr;
  kprintf("\nadcinit: Value of %10x", devptr->dvcsr);

  // configure the ADC control register.
  kprintf("\nadcinit: address of adc control %10x", &(adc_csrptr->ctrl));
  // set the ADC mode to non-touch screen mode.
  adc_csrptr->ctrl &= ADC_SETUP;
  kprintf("\nadcinit: value of adc control after mode %10x", adc_csrptr->ctrl);
  adc_csrptr->ctrl |= ADC_STEPCONFIG_WRITE_PROTECT_OFF;
  kprintf("\nadcinit: value of adc control after write protect %10x", adc_csrptr->ctrl);

  // configure step enable register.
  // 1. Reset the register. 2. Enable only step 1.
  adc_csrptr->step_enable &= ADC_RESET;
  adc_csrptr->step_enable |= ADC_STEP_ENABLE_1;

  //step configuration.
  // value of step 1 config register.
  kprintf("\nadcinit: value of adc step config %10x", adc_csrptr->step[0].step_config);
  // reseting step config 1. Reset changes step to all required values.
  adc_csrptr->step[0].step_config &= ADC_RESET;
  kprintf("\nadcinit: value of adc step config after reset %10x", adc_csrptr->step[0].step_config);

  // enable selected channnel.
  switch(channel_select)
  {
    case 0:
    {
        adc_csrptr->step[0].step_config |= STEP_CHANNEL_0;
        break;
    }
    case 1:
    {
        adc_csrptr->step[0].step_config |= STEP_CHANNEL_1;
        break;
    }
    case 2:
    {
        adc_csrptr->step[0].step_config |= STEP_CHANNEL_2;
        break;
    }
    case 3:
    {
        adc_csrptr->step[0].step_config |= STEP_CHANNEL_3;
        break;
    }
    case 4:
    {
        adc_csrptr->step[0].step_config |= STEP_CHANNEL_4;
        break;
    }
    case 5:
    {
        adc_csrptr->step[0].step_config |= STEP_CHANNEL_5;
        break;
    }
    case 6:
    {
        adc_csrptr->step[0].step_config |= STEP_CHANNEL_6;
        break;
    }
    case 7:
    {
        adc_csrptr->step[0].step_config |= STEP_CHANNEL_7;
        break;
    }
  }


  // clearing interrupt status.
  adc_csrptr->irq_status &= ADC_RESET;
  // enabling FIFO0 threshold interrupt.
  adc_csrptr->irq_enable_set &= ADC_RESET;
  adc_csrptr->irq_enable_set |= THRESHOLD_INTERRUPT;

  // setting pin direction for GPIO power on.
  GPIO_set_dir(2, GPIO_PIN_10, DIR_OUT);

  // setup interrupt handler.
  set_evec(devptr->dvirq, (uint32)devptr->dvintr);
  semADC = semcreate(0);

  kprintf("\nadcinit: End\n");
  return OK;
}
