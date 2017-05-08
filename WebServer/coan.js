/*
var coap        = require('coap')
var server      = coap.createServer()

server.on('request', function(req, res) {
  res.end('Hello ' + req.url.split('/')[1] + '\n')
})

// the default CoAP port is 5683
server.listen(function() {
  console.log("coap server listening on coap://[::1]:5683");
})
*/

var coap = require('coap')
var server = coap.createServer()

// registerFormat('application/json', 50)

var responseArray = {
  status: 500,
  action: 'unknown'
};

server.on('request', function (req, res) {

  var request_name = req.url.split('/')[1];

  process.stdout.write('> action detected: ' + request_name + '\n')
  process.stdout.write(' -> data: ' + req.payload + '\n')

  if (request_name == 'message') {
    responseArray.status = 200;
    responseArray.action = 'message';
  }

  //req.write(JSON.stringify(responseArray));
  res.end('ok');

})

server.listen(function () {

  process.stdout.write('Server is running..\n');
  process.stdout.write('Listening on Port 5683.\n\n');

  var req = coap.request('coap://localhost/message')
  
  req.on('response', function(res) {


    res.pipe(process.stdout)
    res.on('end', function() {
      process.stdout.write('+++ response +++\n');
      process.exit(0)
    })

  })
  //req.end()

})