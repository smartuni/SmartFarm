var coap        = require('coap')
var server      = coap.createServer()

server.on('request', function(req, res) {
  res.end('Hello ' + req.url.split('/')[1] + '\n')
})

// the default CoAP port is 5683
server.listen(function() {
  console.log("coap server listening on coap://[::1]:5683");
})