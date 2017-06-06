// CoAP
var coap = require('coap')

// custom functions
var userFunctions = require('./functions/user')

// Server
var server = coap.createServer(
	{
		type: 'udp6'
	},
	function(req, res) {
		"use strict";
		res.setHeader('Content-Type', 'application/json');
	}
);

// Antwort JSON-Objekt
var responseArray;

// Handler
server.on('request', function (req, res) {
	"use strict";
	
	try {
		
		var isRequest = false;
		
		responseArray = {
			status 	: 500,
			action 	: 'unknown',
			error	: 'unknown action'
		};

		// Request Titel
		var urlArray = req.url.split('/');

		var request_name = '';
		if(urlArray.length > 1) {

			request_name = urlArray[1];

			// Debug Output
			console.log("> action detected: " + request_name);

		}

		var request_data = '';
		if(urlArray.length > 2) {
			request_data = urlArray[2];
		}

		// Payload: Input vom Clienten
		//console.log(" -> data: " + req.payload + "\n");

		// Requests
		if (request_name === 'getusers') {

			if(typeof userFunctions.getUserCount === "undefined") {
				throw(" > undefined function");
			} else {
				isRequest = true;
			}
			
			userFunctions.getUserCount(function(error, count) {

				try {
					
					if(error) {
						throw(error);
					} else {

						responseArray.status = 200;
						responseArray.action = request_name;
						responseArray.data = count;
						responseArray.error = '';

						console.log(" -> getUser response: 200 - " + count);

						res.end(JSON.stringify(responseArray));

					}

				} catch(ex) {
					console.log(ex);
				}

			});

		} if (request_name === 'getuser' && request_data !== '') {

			if(typeof userFunctions.getUser === "undefined") {
				throw(" > undefined function");
			} else {
				isRequest = true;
			}
			
			userFunctions.getUser(parseInt(request_data, 10), function(status, data) {

				try {
					
					responseArray.status = status;
					responseArray.action = request_name;
					responseArray.value = request_data;
					responseArray.data = data;

					if(status === 200) {
						responseArray.error = '';
					}

					console.log(" -> getUser response: " + status + " - " + data);

					res.end(JSON.stringify(responseArray));

				} catch(ex) {
					console.log(ex);
				}

			});

		} else {
			
			if(!isRequest) {
				console.log(" > unknown action");
				res.end(JSON.stringify(responseArray));
			}
			
		}

	} catch (ex) {
		console.log(ex);
		res.end(JSON.stringify(responseArray));
	}
	
});

server.on('error', function(e) {
	"use strict";
	console.log(e);
});

// Listener
server.listen(function () {
	"use strict";

	console.log("Server is running..");
	console.log("Listening on Port 5683.\n");

});