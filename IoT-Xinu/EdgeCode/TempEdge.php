<?php
 /* Authors: Parth Rampal, Pushparaj Britto, Harsh Shah, Prashant Mishra, Rohan Patil	
  * Last Modified: 26-Nov-2016
  * Purpose: Prepared for COP5615 DOS IOT Project
  * Description: 
  *
  *	This script defines the IoT Edge with which forms the bridge between the
  * cloud and the IoT End Devices (Beagle Bone Black) with Temp Sensor
  *
 */
 
 //Reduce errors
 header('Access-Control-Allow-Origin: *');
 date_default_timezone_set('America/New_York');
 error_reporting(~E_WARNING);
  /*	Open Log File	*/
  $frequency = $_GET["frequency"];
 ///fwrite($log,date("Y-F-d H:i:s:u",time())." > IFrequency: $frequency\n");
 /*	Get Value of timeout	*/
 $timeout = $_GET["timeout"];
 //fwrite($log,date("Y-F-d H:i:s:u",time())." > ITimeout: $timeout\n");
  /*	Get Value of precision	*/
  $precision = $_GET["precision"];
 //fwrite($log,date("Y-F-d H:i:s:u",time())." > IPrecision: $precision\n");
 

 exec("pkill -1 -f /var/www/html/pollbbb.php");
 exec("php /var/www/html/pollbbb.php $frequency $timeout $precision &",$result,$var);
 echo	"Result: $var <br>";
 $turnOffLed = file_get_contents("http://192.168.2.7/LEDEdge.php?LED=0");
 echo "$turnOffLed <br/>";
 print_r($result);
 
 
?>
