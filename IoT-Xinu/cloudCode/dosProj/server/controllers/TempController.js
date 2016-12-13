var restful = require('node-restful');

module.exports = function(app, route){
  console.log('controller');

  var rest = restful.model(
    'temp',
    app.models.temp
  ).methods(['get', 'post', 'put', 'delete']);

  rest.register(app, route);

  return function(req, res, next){
    console.log('controller code');
    
    next();
  };
}
