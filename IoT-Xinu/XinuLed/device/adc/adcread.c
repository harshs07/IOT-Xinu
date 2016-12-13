#include <xinu.h>
#include <tempRead-ddl-out>

devcall adcread(struct dentry	*devptr, char	*buffer, uint32 count)
{
  kprintf("\nadcread: start");
  uint32 *temp = (uint32 *)buffer;
  uint32 voltage = 0;
  float voltage1 = 0;

  kprintf("\nadcread before waiting");
  wait(semADC);
  kprintf("\nadcread after waiting");
  voltage = tempVoltage;
  kprintf("\nadcread: value received is %d", voltage);
  voltage1 = CalculateTemp(voltage);
  //voltage1 = ((((voltage * 1.0) / 4095.0)* 1800) - 500) / 10;
  //kprintf("\nadcread: volatge received:- %d", voltage1);
  *temp = (uint32)(voltage1 * 1000);
  kprintf("\nadcread: temp value received:- %d", *temp);

  kprintf("\nadcread: end");
  return OK;
}
