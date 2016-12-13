<?php
 /* Authors: Parth Rampal, Pushparaj Britto, Harsh Shah, Prashant Mishra, Rohan Patil	
  * Last Modified: 26-Nov-2016
  * Purpose: Prepared for COP5615 DOS IOT Project
  * Description: 
  *
  *	This script defines the IoT Edge with which forms the bridge between the
  * cloud and the IoT End Devices (Beagle Bone Black) with LED
  *
 */
 header('Access-Control-Allow-Origin: *');
 //Reduce errors
 error_reporting(~E_WARNING);
date_default_timezone_set('America/New_York');
 /*	Open Log File	*/
 $log = fopen("LED_Edge.log", "a+") or die("Unable to open file!");
 fwrite($log, "\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
 fwrite($log,date("Y-F-d H:i:s",time())." > Edge Called.\n");
 
 /*	BBB will be sent the LED Code	*/
 /*	Get Value of LED Code	*/
 $ledt = $_GET["LED"];
 fwrite($log,date("Y-F-d H:i:s",time())." > LED Code: $ledt \n");

 if($ledt <0 || $ledt >2)	/*	If LED value is invalid, reset to 0	*/
 {
	 $ledt = 0;
	 fwrite($log,date("Y-F-d H:i:s",time())." > LED Code Reset to 0.\n");
 }

 /*	Read configuration file	*/
 
 $conffile = "LEDEdge-BBBLED.json";
 fwrite($log,date("Y-F-d H:i:s",time())." > Reading conf file.\n");
 
 $conf = file_get_contents($conffile);
 $conf2 = file_get_contents($conffile);
 $confarrayr =json_decode($conf2,true);

 $ledoff = $confarrayr['parameters'][0]['name'];
 fwrite($log,date("Y-F-d H:i:s",time())." > ledoff: $ledoff.\n");
 $ledred = $confarrayr['parameters'][1]['name'];
 fwrite($log,date("Y-F-d H:i:s",time())." > ledred: $ledred.\n");
 $ledblue = $confarrayr['parameters'][2]['name'];
 fwrite($log,date("Y-F-d H:i:s",time())." > ledblue: $ledblue.\n");
 $BBBip =  $confarrayr['ip'];
 fwrite($log,date("Y-F-d H:i:s",time())." > BBBip: $BBBip.\n");
 $BBBport = $confarrayr['port'];
 fwrite($log,date("Y-F-d H:i:s",time())." > BBBport: $BBBport.\n");
 
 if($ledt==0)
 {
	 $led = $ledoff;
 }
 if($ledt==1)
 {
	 $led = $ledred;
 }
 if($ledt==2)
 {
	 $led = $ledblue;
 }
 
/*	Create a UDP socket	*/
if(!($sock = socket_create(AF_INET, SOCK_DGRAM, 0)))
{
    $errorcode = socket_last_error();
    $errormsg = socket_strerror($errorcode);
     fwrite($log,date("Y-F-d H:i:s",time())." > Couldn't create socket: [$errorcode] $errormsg .\n");
	 curl_close($curl);
	 fwrite($log,date("Y-F-d H:i:s",time())." > Curl Closed.\n");
	 socket_close($sock);
	 fwrite($log,date("Y-F-d H:i:s",time())." > Socket Closed.\n");
	fwrite($log,date("Y-F-d H:i:s",time())." > =-EOE-=\n");
	fclose($log);
    die("Couldn't create socket: [$errorcode] $errormsg \n");
}

 fwrite($log,date("Y-F-d H:i:s",time())." > Socket Created.\n"); 
 echo "Socket created <br>";

 
/*	 Bind the source address to localost port 33100	*/
if( !socket_bind($sock, "0.0.0.0" , 33100) )
{
    $errorcode = socket_last_error();
    $errormsg = socket_strerror($errorcode);
	fwrite($log,date("Y-F-d H:i:s",time())." > Socket Already bound. Continuing...\n");
}
 fwrite($log,date("Y-F-d H:i:s",time())." > Socket bind OK.\n");
 echo "Socket bind OK <br>";
 
	$y=poll(); 		/*	Poll BBB	*/
 
/*	Poll() sends UDP packet to BBB to get the temperature value and receives the value from the BBB.	*/
 
 function poll()
 {
	 $remote_ip =$GLOBALS['BBBip'];	/*	IP Address of BBB	*/
	 $remote_port = $GLOBALS['BBBport'];			/*	Port number of BBB	*/
	 $rip = '';
	 $rport = 0;
	 
	 fwrite($GLOBALS['log'],date("Y-F-d H:i:s",time())." > Polling BBB...\n");
	 socket_sendto($GLOBALS['sock'], $GLOBALS['led'], 100 , 0 , $remote_ip , $remote_port);	/*	Send UDP packet to BBB	*/
	 echo $GLOBALS['led']." <br>";
	 fwrite($GLOBALS['log'],date("Y-F-d H:i:s",time())." > UDP ".$GLOBALS['led']." sent to BBB.\n");
	 /*	Blocking Receive for UDP packet from BBB	*/
	 
	 $r = socket_recvfrom($GLOBALS['sock'], $buf, 2, 0, $rip, $rport);	
	 echo "$buf received with size $r<br> from IP: $rip , port: $rport";
	 if(!strcmp($buf,"OK"))
	 {
		 echo "<br>LED OK<br>";
		 fwrite($GLOBALS['log'],date("Y-F-d H:i:s",time())." > LED OK.\n");
	 }
	 else
	 {
		 echo "<br>LED Fail<br>";
		 fwrite($GLOBALS['log'],date("Y-F-d H:i:s",time())." > LED Fail.\n");
	 }
	 
 }
 
/*	Close Socket	*/
socket_close($sock);
fwrite($log,date("Y-F-d H:i:s",time())." > Socket Closed.\n");
fwrite($log,date("Y-F-d H:i:s",time())." > =-EOE-=\n");
/*	Close Log file	*/
fclose($log);
?>
