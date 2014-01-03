// express.js stuff
var express = require('express');
var app = express();
app.use('/public', express.static(__dirname + '/public'))
  .use(express.bodyParser());

app.all('/', function(req, res, next) {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "X-Requested-With");
  next();
 });

app.get('/', function(req, res){
	res.sendfile('public/index.html');
});

var port = 3000;
app.listen(port);
console.log('Listening on port', port);
