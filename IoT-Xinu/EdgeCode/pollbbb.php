<?php

 header('Access-Control-Allow-Origin: *');
 date_default_timezone_set('America/New_York');
 error_reporting(~E_WARNING);
 $log = fopen("Temperature_Edge.log", "a+") or die("Unable to open log file!");
 fwrite($log, "\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
 fwrite($log,date("Y-F-d H:i:s:u",time())." > Edge Called.\n");
  
 /*	BBB will be polled Every $frequency seconds for $timeout seconds	*/
 /*	Get Value of Frequency	*/
 $frequency = $argv[1];
 fwrite($log,date("Y-F-d H:i:s:u",time())." > Frequency: $frequency\n");
 /*	Get Value of timeout	*/
 $timeout = $argv[2];
 fwrite($log,date("Y-F-d H:i:s:u",time())." > Timeout: $timeout\n");
  /*	Get Value of precision	*/
  $precision = $argv[3];
 fwrite($log,date("Y-F-d H:i:s:u",time())." > precision: $precision\n");
 
 $count = 0;
 $url = "192.168.2.3:4000/temp";	/*	URL of Cloud Server	*/
 fwrite($log,date("Y-F-d H:i:s:u",time())." > Cloud IP being used: $url\n");
 

 if($frequency <= 0)	/*	If frequency value is invalid, reset to 1	*/
 {
	 $frequency = 1;
	 fwrite($log,date("Y-F-d H:i:s:u",time())." > Frequency Reset to 1.\n");
 }
 if($timeout <= 0)		/*	If timout value is invalid, reset to 1	*/
 {
	 $timeout = 1;
	 fwrite($log,date("Y-F-d H:i:s:u",time())." > Timeout Reset to 1.\n");
 }
 
 if($precision != 1 && $precision != 2 && $precision != 3)
 {
	 $precision = 1;
	 fwrite($log,date("Y-F-d H:i:s:u",time())." > precision Reset to 1.\n");
 }
 
 
  $conffile = "TempEdge-BBBTemp.json";
 fwrite($log,date("Y-F-d H:i:s",time())." > Reading conf file.\n");
 $conf = file_get_contents($conffile);
 $conf2 = file_get_contents($conffile);
 $confarrayr =json_decode($conf2,true);
 $temp1 = $confarrayr['parameters'][0]['name'];
 fwrite($log,date("Y-F-d H:i:s",time())." > ledoff: $ledoff.\n");
 $temp2 = $confarrayr['parameters'][1]['name'];
 fwrite($log,date("Y-F-d H:i:s",time())." > ledred: $ledred.\n");
 $temp3 = $confarrayr['parameters'][2]['name'];
 fwrite($log,date("Y-F-d H:i:s",time())." > ledblue: $ledblue.\n");
 $BBBip =  $confarrayr['ip'];
 fwrite($log,date("Y-F-d H:i:s",time())." > BBBip: $BBBip.\n");
 $BBBport = $confarrayr['port'];
 fwrite($log,date("Y-F-d H:i:s",time())." > BBBport: $BBBport.\n");
 
 
 
  $curl = curl_init($url);	/*	Intialize Curl library to create HTTP POST requests	*/
 fwrite($log,date("Y-F-d H:i:s:u",time())." > Curl initialized.\n");
 
 curl_setopt($curl, CURLOPT_HEADER, false);
 curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
 curl_setopt($curl, CURLOPT_HTTPHEADER,array("Content-type: application/json"));
 curl_setopt($curl, CURLOPT_POST, true);
 
/*	Create a UDP socket	*/
if(!($sock = socket_create(AF_INET, SOCK_DGRAM, 0)))
{
    $errorcode = socket_last_error();
    $errormsg = socket_strerror($errorcode);
     fwrite($log,date("Y-F-d H:i:s:u",time())." > Couldn't create socket: [$errorcode] $errormsg .\n");
	 curl_close($curl);
	 fwrite($log,date("Y-F-d H:i:s:u",time())." > Curl Closed.\n");
	 socket_close($sock);
	 fwrite($log,date("Y-F-d H:i:s:u",time())." > Socket Closed.\n");
	fwrite($log,date("Y-F-d H:i:s:u",time())." > =-EOE-=\n");
	fclose($log);
    die("Couldn't create socket: [$errorcode] $errormsg \n");
}

 fwrite($log,date("Y-F-d H:i:s:u",time())." > Socket Created.\n"); 
 echo "Socket created <br>";

 
/*	 Bind the source address to localost port 33000	*/
if( !socket_bind($sock, "0.0.0.0" , 33000) )
{
    $errorcode = socket_last_error();
    $errormsg = socket_strerror($errorcode);
    //socket_close($sock);
	fwrite($log,date("Y-F-d H:i:s:u",time())." > Socket Already bound. Continuing...\n");
	//fclose($log);
    //die("Could not bind socket : [$errorcode] $errormsg \n");
}
else
{
 fwrite($log,date("Y-F-d H:i:s:u",time())." > Socket bind OK.\n");
 echo "Socket bind OK <br>";
}
 
 while($count<$timeout)		/*	While timout has not been reached.	*/
 {
	$x=poll(); 		/*	Poll BBB	*/
	$y=(float)($x/1000.0);
	fwrite($log,date("Y-F-d H:i:s:u",time())." > Temperature received = $y degrees.\n");
	echo "$y degrees <br>";
	/*	Add temperature inside HTTP POST Request	*/
	$data = array('temp' => (float)$y);
	$content = json_encode($data);	/*	JSON Encode the POST Request	*/
	fwrite($log,date("Y-F-d H:i:s:u",time())." > JSON encoded.\n");
	/*	Prepare HTTP POST JSON Request	*/
	curl_setopt($curl, CURLOPT_POSTFIELDS, $content);
	fwrite($log,date("Y-F-d H:i:s:u",time())." > HTTP POST Request generated.\n");
	/*	Send HTTP Request to Cloud	*/
	///*
	$json_response = curl_exec($curl);
	$status = curl_getinfo($curl, CURLINFO_HTTP_CODE);
	fwrite($log,date("Y-F-d H:i:s:u",time())." > HTTP POST Request sent to cloud with status: $status\n");
	//*/
	/*	If Request to Cloud failed, Retry.	*/
	///*
	if ( $status != 201 ) 
	{
		fwrite($log,date("Y-F-d H:i:s:u",time())." > Error: call to URL: $url failed with status: $status, response: $json_response, curl_error: " . curl_error($curl) . ", curl_errno: " . curl_errno($curl)."\n");
		fwrite($log,date("Y-F-d H:i:s:u",time())." > Reconnecting to cloud..\n");
		$json_response = curl_exec($curl);
		$status = curl_getinfo($curl, CURLINFO_HTTP_CODE);
		fwrite($log,date("Y-F-d H:i:s:u",time())." > HTTP POST Request sent to cloud with status: $status\n");
	}
	//*/
	/*	If retry failed, continue to next itteration..	*/
	///*
 if ( $status != 201 ) 
 {
	 fwrite($log,date("Y-F-d H:i:s:u",time())." >  Error: Retry Failed. Call to URL: $url failed with status: $status, response: $json_response, curl_error: " . curl_error($curl) . ", curl_errno: " . curl_errno($curl)."\n");
	echo "Failed attempt. Temp: $y<br>";
	fwrite($log,date("Y-F-d H:i:s:u",time())." > Sleeping for $frequency seconds...\n");
	sleep($frequency);
	$count=$count+$frequency; 
	fwrite($log,date("Y-F-d H:i:s:u",time())." > Count is $count .\n");
    //die("Error: call to URL $url failed with status $status, response $json_response, curl_error " . curl_error($curl) . ", curl_errno " . curl_errno($curl));
	continue;
 } //*/
	echo "$y degrees <br>";
	fwrite($log,date("Y-F-d H:i:s:u",time())." > Sleeping for $frequency seconds...\n");
	sleep($frequency);	/*	Sleep for $frequency seconds.	*/
	$count=$count+$frequency; 	/*	Increment Time Counter by $fruency seconds.	*/
	fwrite($log,date("Y-F-d H:i:s:u",time())." > Count is $count .\n");
 }
 
/*	Poll() sends UDP packet to BBB to get the temperature value and receives the value from the BBB.	*/
 
 function poll()
 {
	 $remote_ip = $GLOBALS['BBBip'];	/*	IP Address of BBB	*/
	 $remote_port = $GLOBALS['BBBport'];			/*	Port number of BBB	*/
	 $rip = '';
	 $rport = 0;
	 
	 fwrite($GLOBALS['log'],date("Y-F-d H:i:s:u",time())." > Polling BBB...\n");
	 if($GLOBALS['precision']==1)
	 {
		socket_sendto($GLOBALS['sock'], $GLOBALS['temp1'], 100 , 0 , $remote_ip , $remote_port);	/*	Send UDP packet to BBB	*/
		fwrite($GLOBALS['log'],date("Y-F-d H:i:s:u",time())." > UDP ".$GLOBALS['temp1']." sent to BBB.\n");
	 }
	 if($GLOBALS['precision']==2)
	 {
		socket_sendto($GLOBALS['sock'], $GLOBALS['temp2'], 100 , 0 , $remote_ip , $remote_port);	/*	Send UDP packet to BBB	*/
		fwrite($GLOBALS['log'],date("Y-F-d H:i:s:u",time())." > UDP ".$GLOBALS['temp2']." sent to BBB.\n");
	 }
	 if($GLOBALS['precision']==3)
	 {
		socket_sendto($GLOBALS['sock'], $GLOBALS['temp3'], 100 , 0 , $remote_ip , $remote_port);	/*	Send UDP packet to BBB	*/
		fwrite($GLOBALS['log'],date("Y-F-d H:i:s:u",time())." > UDP ".$GLOBALS['temp3']." sent to BBB.\n");
	 }
	 
	 $r = socket_recvfrom($GLOBALS['sock'], $buf, 512, 0, $rip, $rport);	/*	Blocking Receive for UDP packet from BBB	*/
	 
	 fwrite($GLOBALS['log'],date("Y-F-d H:i:s:u",time())." > $r bytes of Temperature received: $buf degrees from IP: $rip , port: $rport.\n");	 
	 //static $x =30;
	 //return ++$x;
	 return (float)$buf;
 }
 
 /*
//Do some communication, this loop can handle multiple clients
while(1)
{
	for($i=0;$i<frequency)
    echo "Waiting for data ... \n";
     
    //Receive some data
    $r = socket_recvfrom($sock, $buf, 512, 0, $remote_ip, $remote_port);
    echo "$remote_ip : $remote_port -- " . $buf.PHP_EOL;
     
    //Send back the data to the client
	
    socket_sendto($sock, "OK " . $buf , 100 , 0 , $remote_ip , $remote_port);
}
 */
	/*	Close Curl	*/
	curl_close($curl);
	fwrite($log,date("Y-F-d H:i:s:u",time())." > Curl Closed.\n");
	//$response = json_decode($json_response, true);
	/*	Close Socket	*/
	socket_close($sock);
	fwrite($log,date("Y-F-d H:i:s:u",time())." > Socket Closed.\n");
	fwrite($log,date("Y-F-d H:i:s:u",time())." > =-EOE-=\n");
	//file_put_contents($process_file, "");
	//fwrite($log,date("Y-F-d H:i:s:u",time())." > Process file cleaned.\n");
	/*	Close Log file	*/
	fclose($log);
?>
