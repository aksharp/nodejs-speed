// import fib.js
var fib = require('./fib.js').functions.fib;
var rndArr = require('./fib.js').functions.rndArr;
var arrToFib = require('./fib.js').functions.arrToFib;

// util.js
var execF2 = require('./util.js').execF2;

// express.js stuff
var express = require('express');
var app = express();
app.use(express.bodyParser());

// calculate js
app.post('/calculate-js', function(req, res){
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "X-Requested-With");
  var arr = req.body.arr;
  var pocJsRes = execF2(arrToFib, arr, fib, "js");
  res.json(pocJsRes);
});

var port = 5000;
app.listen(port);
console.log('Listening on port', port);
