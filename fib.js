var fib = function(n) {
  if (n <= 1) {
     return n;
  } else {
     return fib(n-1) + fib(n-2);
  }
};

var rndArr = function(arrSize, maxNum) {
  var arr = [];
  for (var i=0; i<arrSize; ++i) {
    arr[i] = Math.floor((Math.random() * maxNum) + 1);
  } 
  return arr;
};

var arrToFib = function(arr, f) {
  var res = [];
  for (var i=0; i<arr.length; ++i) {
    res[i] = f(arr[i]);
  }
  return res;
};

var res = {
  'fib': fib,
  'rndArr': rndArr,
  'arrToFib': arrToFib
};

exports.functions = res;
