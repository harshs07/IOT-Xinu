var express = require('express');
var mongoose = require('mongoose');
var bodyParser = require('body-parser');
var methodOverride = require('method-override');
var _ = require('lodash');

// Create the app
var app = express();

//Middleware for REST API - ?
app.use(bodyParser.urlencoded({extended:true}));
app.use(bodyParser.json());
app.use(methodOverride('X-HTTP-Method-Override'));

// CORS Support
app.use(function(req, res, next) {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Methods', 'GET,PUT,POST,DELETE,OPTIONS');
  res.header('Access-Control-Allow-Headers', "Origin, X-Requested-With, Content-Type, Accept");
  next();
});

// Connect to MongoDB
mongoose.connect('mongodb://localhost/dosapp');
mongoose.connection.once('open', function(){

    // Load the models
  app.models = require('./models/index');

  // Load the routes
  var routes = require('./routes');
  _.each(routes, function(controller, route){
    app.use(route, controller(app,route));
  });

  console.log('Listening on port 4000');
  app.listen(4000);
});
