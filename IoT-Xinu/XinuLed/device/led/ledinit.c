#include <xinu.h>

devcall ledinit(struct dentry	*devptr)
{
  // setting up GPIO pins to receive output.
  /*GPIO_set_dir(1, GPIO_PIN_13, DIR_OUT);
	GPIO_set_dir(1, GPIO_PIN_12, DIR_OUT);
  GPIO_set_dir(2, GPIO_PIN_10, DIR_OUT);*/
  #include <ledInit-ddl-out>

  //creating semaphore for coordination between upper and lower level.
  semLedControl = semcreate(0);

  //Initializing the coordination variable.
  // set to 2 as 0 - Off, 1 - On.
  ledControl = 0xFFFF;

  //setting up the interrupt vextor.
  set_evec(devptr->dvirq, (uint32)devptr->dvintr);

  return OK;
}
