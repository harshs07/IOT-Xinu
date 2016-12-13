'use strict';

/**
 * @ngdoc function
 * @name clientApp.controller:TempCtrl
 * @description
 * # TempCtrl
 * Controller of the dosApp
 */
angular.module('dosApp')
  .controller('TempCtrl', function ($scope, $http, Temp) {
    var chart, runTimer;
    console.log(CONFIG);
    var edgeIP = CONFIG.edgeIP;
    Highcharts.setOptions({
      global : {
        useUTC : false
      }
    });
    $scope.selectedInterval = CONFIG.temp.defaultFrequency;  // Default Temp Request frequency - 3 seconds
    $scope.led = CONFIG.led.defaultledMode; // Default Led Mode - Off
    $scope.mode = CONFIG.temp.defaultEnergyMode;; // Default Power Mode - Energy Efficient
    $scope.modes = ["", "Energy Efficient", "Balanced", "High Performance"];
    $scope.threshold = CONFIG.temp.threshold;

    $scope.currentMode = $scope.modes[$scope.mode];

    $scope.triggerLED = CONFIG.led.triggerOption;

    $scope.sendLED = function(num){
      var ledVal = num || $scope.led;
      console.log("sendLED " + ledVal);
      var uri = "http://"+edgeIP+"/LEDEdge.php?LED="+ledVal;
      console.log("uri " + uri);
      $http({
      method: "GET",
      url: uri,
      }).success(function(response) {
           console.log(response); //get the echo value from php page
      }).error(function(response) {
            console.log(response);
      });
    };

    $scope.setMode = function(){
      $scope.currentMode = $scope.modes[$scope.mode];
      console.log("Set Mode " + $scope.mode);
      var uri = "http://"+edgeIP+"/TempEdge.php?frequency="+$scope.selectedInterval+"&timeout=60"+"&precision="+$scope.mode;
      console.log("uri " + uri);
       $http({
       method: "GET",
       url: uri,
       }).success(function(response) {
            console.log(response); //get the echo value from php page
       }).error(function(response) {
             console.log(response);
       });
    };

  	$scope.resetTimer = function(){
      console.log("Temp Frequency " + $scope.selectedInterval);
      var uri = "http://"+edgeIP+"/TempEdge.php?frequency="+$scope.selectedInterval+"&timeout=60"+"&precision="+$scope.mode;
      console.log("uri " + uri);
       $http({
       method: "GET",
       url: uri,
       }).success(function(response) {
            console.log(response); //get the echo value from php page
       }).error(function(response) {
             console.log(response);
       });
  	};

    $scope.data = Temp.getList().$object;

    function checkDirty(){

        $scope.data = Temp.getList().$object;
        clearInterval(runTimer);
        setTimeout(function(){
          var tempObj = checkLatestTemp();
          if(angular.isDefined(tempObj) && angular.isDefined(tempObj.tmp)){
            console.log('New Node Added ' + tempObj.tmp);
            $scope.tempData.push(tempObj);
            console.log("Trigger LED: " + $scope.triggerLED);
            console.log("Threshold: " + $scope.threshold);
            if($scope.triggerLED){
              if(tempObj.tmp > $scope.threshold){
                $scope.sendLED(1);
              }
              else{
                $scope.sendLED(2);
              }
            }
            chart.series[0].addPoint([tempObj.time,tempObj.tmp],true,true);
          }
        },200);
        runTimer = setInterval(checkDirty,$scope.selectedInterval * 1000);
    }

    function checkLatestTemp(){
      var length = $scope.data.length;
      if(length === 0){
        return {};
      }
      var currentSample = $scope.data[length - 1];
      var time = new Date(currentSample.time);
      var sampleTime = time.getTime();
      if(sampleTime !== $scope.lastRecordedTime){
        $scope.lastRecordedTime = sampleTime;
        return {
          tmp : currentSample.temp,
          time : sampleTime
        };
      }
      else {
        return {};
      }
    }

    function createSeries(){
      var length = $scope.data.length;
      var initData = [];
      angular.forEach($scope.data, function(value,key){
        var time = new Date(value.time);
        if(key >= length - 10){
          initData.push({
                         x: time.getTime(),
                         y: value.temp
                     });
          $scope.lastRecordedTime = time.getTime();
          $scope.lastRecordedTemp = value.temp;
        }
      });
      return initData;
    }

    setTimeout(function() {
      $scope.tempData = createSeries();
      console.log($scope.tempData);
      initChart();
      runTimer = setInterval(checkDirty,$scope.selectedInterval * 1000);
    }, 500);

    $scope.addTemp = function(tmp){
        console.log(tmp);

         var postData = {
           "temp" : tmp
         };
         Temp.post(postData).then(function(){
           console.log('Posted Temp');
         });


      // var time = (new Date()).getTime();
      // console.log(chart.series[0]);
      // chart.series[0].addPoint([time,tmp],true,true);
    };

  	// function getRandTemp(){
  	// 	var max = 30;
  	// 	var min = 10;
  	// 	return Math.floor(Math.random()*(max-min+1)+min);
  	// }

  	// Replace with First five/ten values of temp sensor
  	// $scope.getData = function(){
  	// 	var initData = [];
    //
  	// 	//static Data
  	// 	for (var i = -10; i <= 0; i += 1) {
    //           initData.push({
    //               x: (new Date()).getTime() + (i * 5000),
    //               y: getRandTemp()
    //           });
    //       }
    //   console.log(initData);
  	// 	return initData;
  	// };

    function initChart(){
      console.log('initChart');
      Highcharts.chart('chartContainer', {
            chart: {
                type: 'spline',
                animation: Highcharts.svg, // don't animate in old IE
                marginRight: 10,
                events: {
                	load: function () {
                        console.log('load');
                        // set up the updating of the chart each second
                        console.log(this);
                        chart = this;
                        var series = this.series[0];
                        var chartData = series.data;
                        if(angular.isArray(chartData) && chartData.length === 0){
                          console.log(series);
                        	series.setData($scope.tempData);
                        }
                        // setInterval(function () {
                        //     console.log($scope);
                        //     var x = (new Date()).getTime(), // current time
                        //         y = getRandTemp();
                        //     series.addPoint([x, y], true, true);
                        // }, 5000);
                    }
                }
            },
            title: {
                text: 'Temperature vs Time',
                style : {
                	color : '#1894bd'
                }
            },
            xAxis: {
                type: 'datetime',
                tickPixelInterval: 150,
                title : {
                	text: 'Time [hh:mm:ss]',
                	margin : 20,
                	style : {
                    	color : '#a61423'
                    }
                }
            },
            yAxis: {
                title: {
                    text: 'Temperature (°C)',
                    margin : 20,
                    style : {
                    	color : '#a61423'
                    }
                },
                plotLines: [{
                    value: 0,
                    width: 1,
                    color: '#808080'
                }]
            },
            tooltip: {
                formatter: function () {
                    return '<b>' + this.series.name + '</b><br/>' +
                        Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) + '<br/>' +
                        Highcharts.numberFormat(this.y, 3);
                }
            },
            legend: {
                enabled: false
            },
            exporting: {
                enabled: false
            },
            series: [{
                name: 'Temperature Data',
                data: []
            }]
        });
    }

  });
