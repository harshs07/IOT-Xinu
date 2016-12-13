#include <xinu.h>

sid32 semADC;
uint32 tempVoltage;

//volatile uint32 *adc_tsc_clkctrl;
//struct adc_csr *adc_csrptr;

//#define REG(n) (*((volatile uint32*)(n)))

//void adc_init() {

	// enable the CM_WKUP_ADC_TSC_CLKCTRL with CM_WKUP_MODUELEMODE_ENABLE
  //volatile uint32 *adc_tsc_clkctrl = &CM_WKUP_ADC_TSC_CLKCTRL;
	//REG(CM_WKUP_ADC_TSC_CLKCTRL) |= CM_WKUP_MODULEMODE_ENABLE;

	// wait for the enable to complete
	//while(!(REG(CM_WKUP_ADC_TSC_CLKCTRL) & CM_WKUP_MODULEMODE_ENABLE)) {
		// waiting for adc clock module to initialize
		//printf("Waiting for CM_WKUP_ADC_TSC_CLKCTRL to enable with MODULEMODE_ENABLE\n");
	//}
	// software reset, set bit 1 of sysconfig high?
	// make sure STEPCONFIG write protect is off
	//REG(ADC_CTRL) |= ADC_STEPCONFIG_WRITE_PROTECT_OFF;

  //volatile uint32 *adc_control = &ADC_CTRL;

	// set up each ADCSTEPCONFIG for each ain pin
	//REG(ADCSTEPCONFIG1) = 0x00<<19 | STEP_AVG16;
	//REG(ADCSTEPDELAY1)  = (0x0F)<<24;
	// REG(ADCSTEPCONFIG2) = 0x01<<19 | ADC_AVG16;
	// REG(ADCSTEPDELAY2)  = (0x0F)<<24;
	// REG(ADCSTEPCONFIG3) = 0x02<<19 | ADC_AVG16;
	// REG(ADCSTEPDELAY3)  = (0x0F)<<24;
	// REG(ADCSTEPCONFIG4) = 0x03<<19 | ADC_AVG16;
	// REG(ADCSTEPDELAY4)  = (0x0F)<<24;
	// REG(ADCSTEPCONFIG5) = 0x04<<19 | ADC_AVG16;
	// REG(ADCSTEPDELAY5)  = (0x0F)<<24;
	// REG(ADCSTEPCONFIG6) = 0x05<<19 | ADC_AVG16;
	// REG(ADCSTEPDELAY6)  = (0x0F)<<24;
	// REG(ADCSTEPCONFIG7) = 0x06<<19 | ADC_AVG16;
	// REG(ADCSTEPDELAY7)  = (0x0F)<<24;
	// REG(ADCSTEPCONFIG8) = 0x07<<19 | ADC_AVG16;
	// REG(ADCSTEPDELAY8)  = (0x0F)<<24;

	// enable the ADC
	// REG(ADC_CTRL) |= 0x01;
//}

//uint32 analogRead(uint32 ain) {

	// the clock module is not enabled
	// if(REG(CM_WKUP_ADC_TSC_CLKCTRL) & CM_WKUP_IDLEST_DISABLED)
	// 	adc_init();
  //
	// // enable the step sequencer for this pin
	// REG(ADC_STEPENABLE) |= (0x01<<(ain+1));
  //
	// // return the the FIFO0 data register
	// return REG(ADC_FIFO0DATA) & ADC_FIFO_MASK;
//}
