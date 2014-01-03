// execute function with 2 parameters
var execF2 = function(f, p1, p2, fName){
  var before = Date.now();
  var res = f(p1, p2);
  var after = Date.now();
  var elapsed = after - before;
  return {
    "fName": fName,
    "res": res,
    "elapsed": elapsed
  };  
};

exports.execF2 = execF2;
