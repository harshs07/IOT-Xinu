#include <xinu.h>

devcall ledwrite(struct dentry * devptr,
              char * buffer,
              uint32 count)
{
  kprintf("\nledwrite: start");

  //reading Led control value.
  uint32 ledC = *(uint32 *)(buffer);
  kprintf("\nledwrite: Value received is %d", ledC);

  // kickout LED handler.
  kprintf("\nledwrite: interrupt set.");
  irq_set(devptr->dvirq);

  // waiting for LED handler to signal.
  kprintf("\nledwrite: waiting for led handler");
	wait(semLedControl);
  kprintf("\nledwrite: write led on/off");
  ledControl = ledC;

  // end of program.
  kprintf("\nledwrite: end");
  return OK;
}
