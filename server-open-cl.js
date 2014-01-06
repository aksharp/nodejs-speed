// open cl implementation
var arrToFibCpp = require('./cpp-modules/build/Release/fibs').ArrToFib;

// express.js stuff
var express = require('express');
var app = express();
app.use(express.bodyParser());

var execF = function(f, p1, fName){
  var before = Date.now();
  var res = f(p1);
  var after = Date.now();
  var elapsed = after - before;
  return {
    "fName": fName,
    "res": res,
    "elapsed": elapsed
  };  
};

// poc open cl
app.post('/poc-open-cl', function(req, res){
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "X-Requested-With");
  var arr = req.body.arr;
  var pocCppRes = execF(arrToFibCpp, arr, "open-cl");
  res.json(pocCppRes);
});

var port = 4040;
app.listen(port);
console.log('Listening on port', port);
