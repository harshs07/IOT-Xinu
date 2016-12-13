#include <xinu.h>

#include <ledWrite-ddl-out>

void ledhandler(uint32	xnum)
{
	kprintf("\nledhanlder: start");
	GPIO_high(1, GPIO_PIN_12);
	uint32 ledC;
	//intmask mask;


	kprintf("\nledhandler: Interrupt Bit Cleared.");
	irq_clear(xnum);

	kprintf("\nledhandler: signal semLedControl");
	executeTest = 0;
	signal(semLedControl);

	// interrupt already processed.
	// When signal is executed for the first time, it starts executing till
	// signal is called. By the time the first interrupt is called again, the interrupt
	// handler is trigerred again.

	if(executeTest == 1)
	{
		return;
	}
	kprintf("\nledhandler: after signal is called");
	ledC = ledControl;

	WriteLED_Pins(ledC);

	/*if(ledC == 2)
	{
		kprintf("\nledhandler: Led Turned On");
		GPIO_high(1, GPIO_PIN_13);
		GPIO_low(2, GPIO_PIN_10);
	}
	if(ledC == 1)
	{
		kprintf("\nledhandler: Led Turned On");
		GPIO_low(1, GPIO_PIN_13);
		GPIO_high(2, GPIO_PIN_10);
	}
	if(ledC == 0)
	{
		kprintf("\nledhandler: Led Turned Off");
		GPIO_high(1, GPIO_PIN_13);
		GPIO_high(2, GPIO_PIN_10);
	}*/

	executeTest = 1;
	return;
}
