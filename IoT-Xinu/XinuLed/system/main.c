/*  main.c  - main */

#include <xinu.h>


pid32	netpid,ledcontrollerid;
int32 	slot=0;

process ledcontroller(){

	umsg32 msg;
	uint32 buff;
	int32	retval;
	struct	udpentry *udptr;

	while(1){
		msg=receive();
		if(msg==1){
		buff=msg;
		write(LEDDRV,(char*)&buff,1);
		}
		else if(msg==2){
		buff=msg;
		write(LEDDRV,(char*)&buff,1);
		}
		else{
		buff=0;
		write(LEDDRV,(char*)&buff,1);
		}

			udptr = &udptab[slot];
			if(udptr->udstate != UDP_FREE)					// Ensure slot is valid
			{
			retval = udp_sendto(slot,0xC0A80207,33100, "OK",2);
			}
			else								// if slot is not valid
			{
				retval = SYSERR;
				kprintf("No valid slot\n\r");
			}

			if (retval == SYSERR) 						// Slot was invalid, UDP not sent
			{
				kprintf("cannot send UDP\n\r");
			}
			else								// UDP sent
			{
				kprintf("UDP SENT\n\r");
			}
	}

}

process network()
{
	int32 	i, j;
	int32	retval;
	uint32	localip;
	uint32  lent;
	char	buff[200];
	uint32	waitfor = 60;	// milliseconds to wait while receiving UDP packets
	struct	udpentry *udptr;

	resume( (netpid = create(netin, 8192, 50, "netin", 0)) );
	NetData.ipvalid = FALSE;
	retval = getlocalip();
	if (retval == SYSERR)
	{
		panic("Error: could not obtain an IP address\n\r");
	}
	else
	{
		/*	Print IP address in ASCII and Hex	*/
	  kprintf("IP address is %d.%d.%d.%d   %08x\n\r",
		(retval>>24)&0xff, (retval>>16)&0xff, (retval>>8)&0xff,
		 retval&0xff,retval);

		 /*	Print Subnet mask and IP address of router	*/
		slot = udp_register(0, 33100,31000);	// Register slot in udptab
		if (slot == SYSERR)
		{
			kprintf("cannot register 0, 33100, 31000\n\r");
		}

		while (TRUE)
		{
				/*Reset buff to blank*/
				for(i=0;i<200;i++)
				{
					buff[i]=NULL;
				}

			lent = udp_recv(slot,buff,sizeof(buff),waitfor);

			// Message Passing


			j=strcmp("led1",buff);
			if(j==0)
			{
			send(ledcontrollerid,1);
			}

			j=strcmp("led2",buff);
			if(j==0)
			{
			send(ledcontrollerid,2);
			}

			j=strcmp("led0",buff);
			if(j==0)
			{
			send(ledcontrollerid,0);
			}
		}
	}
	return OK;
}

process	main(void)
{
	pid32 networkid;
	open(ETHER0,NULL, NULL);
	resume( (networkid = create(network, 8192, 50, "network", 0)) );
	resume( (ledcontrollerid = create(ledcontroller, 8192, 50, "ledcontroller", 0)));

	return OK;

}
