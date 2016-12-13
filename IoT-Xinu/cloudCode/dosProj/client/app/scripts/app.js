'use strict';

/**
 * @ngdoc overview
 * @name clientApp
 * @description
 * # clientApp
 *
 * Main module of the application.
 */
angular
  .module('dosApp', [
    'ngRoute',
    'highcharts-ng',
    'restangular',
    'ngMaterial'
  ])
  .config(function ($routeProvider, $httpProvider, RestangularProvider) {

    $httpProvider.defaults.useXDomain = true;
    delete $httpProvider.defaults.headers.common['X-Requested-With'];

    RestangularProvider.setBaseUrl(CONFIG.nodeAPI);

    $routeProvider
      .when('/', {
        templateUrl: 'views/main.html',
        controllerAs: 'main'
      })
      .when('/about', {
        templateUrl: 'views/about.html',
        controllerAs: 'about'
      })
      .when('/temp', {
        templateUrl: 'views/temp.html',
        controllerAs: 'temp'
      })
      .otherwise({
        redirectTo: '/'
      });
  }).factory('TempRestangular', function(Restangular) {
    return Restangular.withConfig(function(RestangularConfigurer) {
      RestangularConfigurer.setRestangularFields({
        id: '_id'
      });
    });
  })
  .factory('Temp', function(TempRestangular) {
    return TempRestangular.service('temp');
  });
