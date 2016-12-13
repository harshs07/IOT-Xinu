var mongoose = require('mongoose');

//Schema
var TempSchema = new mongoose.Schema({
        temp : Number,
        time : { type : Date, default: Date.now }
      });

//Return model
module.exports = TempSchema;
