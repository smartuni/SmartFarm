// CoAP
var coap = require('coap')

// custom functions
var userFunctions = require('./functions/user')
var fenceFunctions = require('./functions/fence')
var gateFunctions = require('./functions/gate')
var livestockFunctions = require('./functions/livestock')

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

		if(request_name.match(/User/)) {
						
			if(typeof userFunctions[request_name] === "undefined") {
				responseArray.error = 'function: ' + request_name + ' not found';
				console.log(' > function: ' + request_name + ' not found');
			} else {
				
				isRequest = true;
				
				userFunctions[request_name]([request_data], function(error, data) {

					responseArray.action = request_name;
					
					try {

						if(error) {
							throw(error);
						} else {

							responseArray.status = 200;
							responseArray.data = data;
							responseArray.error = '';

							console.log(" -> " + request_name + " response: 200");
							console.log(" -> Data: " + JSON.stringify(data));

						}

					} catch(ex) {
						console.log(" -> Exception: " + JSON.stringify(ex));
						responseArray.error = ex;
					}
					
					res.end(JSON.stringify(responseArray));

				});
				
			}
			
		} else if(request_name.match(/Gate/)) {
			
			if(typeof gateFunctions[request_name] === "undefined") {
				responseArray.error = 'function: ' + request_name + ' not found';
				console.log(' > function: ' + request_name + ' not found');
			} else {
				
				isRequest = true;
				
				gateFunctions[request_name]([request_data], function(error, data) {
					
					responseArray.action = request_name;
					
					try {

						if(error) {
							throw(error);
						} else {

							responseArray.status = 200;
							responseArray.data = data;
							responseArray.error = '';

							console.log(" -> " + request_name + " response: 200");
							console.log(" -> Data: " + JSON.stringify(data));

						}

					} catch(ex) {
						console.log(" -> Exception: " + JSON.stringify(ex));
						responseArray.error = ex;
					}
					
					res.end(JSON.stringify(responseArray));
					
				});
				
			}
			
		} else if(request_name.match(/Fence/)) {
			
			if(typeof fenceFunctions[request_name] === "undefined") {
				responseArray.error = 'function: ' + request_name + ' not found';
				console.log(' > function: ' + request_name + ' not found');
			} else {
				
				isRequest = true;
				
				fenceFunctions[request_name]([request_data], function(error, data) {
					
					responseArray.action = request_name;
					
					try {

						if(error) {
							throw(error);
						} else {

							responseArray.status = 200;
							responseArray.data = data;
							responseArray.error = '';

							console.log(" -> " + request_name + " response: 200");
							console.log(" -> Data: " + JSON.stringify(data));

						}

					} catch(ex) {
						console.log(" -> Exception: " + JSON.stringify(ex));
						responseArray.error = ex;
					}
					
					res.end(JSON.stringify(responseArray));
					
				});
				
			}
			
		} else if(request_name.match(/Livestock/)) {
			
			if(typeof livestockFunctions[request_name] === "undefined") {
				responseArray.error = 'function: ' + request_name + ' not found';
				console.log(' > function: ' + request_name + ' not found');
			} else {
				
				isRequest = true;
				
				livestockFunctions[request_name]([request_data], function(error, data) {
					
					responseArray.action = request_name;
					
					try {

						if(error) {
							throw(error);
						} else {

							responseArray.status = 200;
							responseArray.data = data;
							responseArray.error = '';

							console.log(" -> " + request_name + " response: 200");
							console.log(" -> Data: " + JSON.stringify(data));

						}

					} catch(ex) {
						console.log(" -> Exception: " + JSON.stringify(ex));
						responseArray.error = ex;
					}
					
					res.end(JSON.stringify(responseArray));
					
				});
				
			}
			
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