/*  main.c  - main */

#include <xinu.h>

pid32	netpid,controllerid;
int32 	slot=0;

process controller(void)
{
	umsg32 msg;
	int32 	i;
	int32	retval;
	struct	udpentry *udptr;
	uint32 ans;
  	char str[20];

	while(1)
{
	msg=receive();
	if(msg==1)
	{
		uint32 mode = 1;
		kprintf("Temp Message 1 received\n");
		open(TMPADC, (char *)&mode, NULL);
		read(TMPADC,(char*)&ans,1);
		close(TMPADC);
	}
	
	if(msg==2)
	{
		uint32 mode = 2;
		kprintf("Temp Message 2 received\n");
		open(TMPADC, (char *)&mode, NULL);
		read(TMPADC,(char*)&ans,1);
		close(TMPADC);
	}

	if(msg==3)
	{
		uint32 mode = 3;
		kprintf("Temp Message 3 received\n");
		open(TMPADC, (char *)&mode, NULL);
		read(TMPADC,(char*)&ans,1);
		close(TMPADC);
	}

			/*Reset buff to blank*/
			for(i=0;i<20;i++)
			{
				str[i]=NULL;
			}

			udptr = &udptab[slot];
			
			if(udptr->udstate != UDP_FREE)					// Ensure slot is valid
			{
			sprintf(str,"%d",ans);
			retval = udp_sendto(slot,0xC0A80207,33000, str,5);
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

process network(){


	int32 	i,j;
	int32	retval;
	uint32  lent;
	char	buff[200];
	uint32	waitfor = 60;	// milliseconds to wait while receiving UDP packets
	struct	udpentry *udptr;

	resume( (netpid = create(netin, 8192, 50, "netin", 0)) );

	NetData.ipvalid = FALSE;
	retval = getlocalip();
	if (retval == SYSERR) {
		panic("Error: could not obtain an IP address\n\r");
	} 
	
	else {
	slot = udp_register(0, 33000,31000);	// Register slot in udptab
	if (slot == SYSERR) {
		kprintf("cannot register 0, 33000, 31000\n\r");
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

		j=strcmp("getTemp1",buff);
		if(j==0)
		send(controllerid,1);

		j=strcmp("getTemp2",buff);
		if(j==0)
		send(controllerid,2);
				
		
		j=strcmp("getTemp3",buff);
		if(j==0)
		send(controllerid,3);	

	}

	}
}

process	main(void)
{
	pid32 networkid;
	open(ETHER0,NULL, NULL);
	resume( (networkid = create(network, 8192, 50, "network", 0)) );
	resume( (controllerid = create(controller, 8192, 50, "controller", 0)));

	return OK;

}
