// c++ implementation
var fibCpp = require('./cpp-modules/build/Release/fib').Fib;
var rndArrCpp = require('./cpp-modules/build/Release/fib').RndArr;
var arrToFibCpp = require('./cpp-modules/build/Release/fib').ArrToFib;

// util.js
var execF2 = require('./util.js').execF2;

// express.js stuff
var express = require('express');
var app = express();
app.use('/public', express.static(__dirname + '/public'))
  .use(express.bodyParser());

// poc cpp
app.post('/poc-cpp', function(req, res){
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "X-Requested-With");
  var arr = req.body.arr;
  var pocCppRes = execF2(arrToFibCpp, arr, fibCpp, "c++");
  res.json(pocCppRes);
});

var port = 4000;
app.listen(port);
console.log('Listening on port', port);
