'use strict';

describe('Controller: TempCtrl', function () {

  // load the controller's module
  beforeEach(module('clientApp'));

  var TempCtrl,
    scope;

  // Initialize the controller and a mock scope
  beforeEach(inject(function ($controller, $rootScope) {
    scope = $rootScope.$new();
    TempCtrl = $controller('TempCtrl', {
      $scope: scope
      // place here mocked dependencies
    });
  }));

  it('should attach a list of awesomeThings to the scope', function () {
    expect(TempCtrl.awesomeThings.length).toBe(3);
  });
});
