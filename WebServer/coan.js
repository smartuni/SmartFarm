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
				console.log(' > userfunction: ' + request_name + ' not found');
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
			
		}
		
		// Requests
		if (request_name === 'getgates') {

			if(typeof gateFunctions.getGateCount === "undefined") {
				throw(" > undefined function");
			} else {
				isRequest = true;
			}
			
			gateFunctions.getGateCount(function(error, count) {

				try {
					
					if(error) {
						throw(error);
					} else {

						responseArray.status = 200;
						responseArray.action = request_name;
						responseArray.data = count;
						responseArray.error = '';

						console.log(" -> getGateCount response: 200 - " + count);

						res.end(JSON.stringify(responseArray));

					}

				} catch(ex) {
					console.log(ex);
				}

			});

		} else if (request_name === 'getgate' && request_data !== '') {

			if(typeof gateFunctions.getGate === "undefined") {
				throw(" > undefined function");
			} else {
				isRequest = true;
			}
			
			gateFunctions.getGate(parseInt(request_data, 10), function(status, data) {

				try {
					
					responseArray.status = status;
					responseArray.action = request_name;
					responseArray.value = request_data;
					responseArray.data = data;

					if(status === 200) {
						responseArray.error = '';
					}

					console.log(" -> getGate response: " + status + " - " + data.data.id);

					res.end(JSON.stringify(responseArray));

				} catch(ex) {
					console.log(ex);
				}

			});

		} else if (request_name === 'togglegate' && request_data !== '') {

			if(typeof gateFunctions.toggleGate === "undefined") {
				throw(" > undefined function");
			} else {
				isRequest = true;
			}
			
			var gate_id = parseInt(request_data, 10);
						
			gateFunctions.toggleGate(gate_id, function(status, data) {

				try {
					
					responseArray.status = status;
					responseArray.action = request_name;
					responseArray.value = request_data;
					responseArray.data = data;

					if(status === 200) {
						responseArray.error = '';
					}

					console.log(" -> toggleGate response: " + status + " - " + data.data.state);

					res.end(JSON.stringify(responseArray));

				} catch(ex) {
					console.log(ex);
				}

			});

		} else if (request_name === 'getfences') {

			if(typeof fenceFunctions.getFenceCount === "undefined") {
				throw(" > undefined function");
			} else {
				isRequest = true;
			}
			
			fenceFunctions.getFenceCount(function(error, count) {

				try {
					
					if(error) {
						throw(error);
					} else {

						responseArray.status = 200;
						responseArray.action = request_name;
						responseArray.data = count;
						responseArray.error = '';

						console.log(" -> getFenceCount response: 200 - " + count);

						res.end(JSON.stringify(responseArray));

					}

				} catch(ex) {
					console.log(ex);
				}

			});

		} else if (request_name === 'getfence' && request_data !== '') {

			if(typeof fenceFunctions.getFence === "undefined") {
				throw(" > undefined function");
			} else {
				isRequest = true;
			}
			
			fenceFunctions.getFence(parseInt(request_data, 10), function(status, data) {

				try {
					
					responseArray.status = status;
					responseArray.action = request_name;
					responseArray.value = request_data;
					responseArray.data = data;

					if(status === 200) {
						responseArray.error = '';
					}

					console.log(" -> getFence response: " + status + " - " + data.data.id);

					res.end(JSON.stringify(responseArray));

				} catch(ex) {
					console.log(ex);
				}

			});

		} else if (request_name === 'getlivestocks') {

			if(typeof livestockFunctions.getLivestockCount === "undefined") {
				throw(" > undefined function");
			} else {
				isRequest = true;
			}
			
			livestockFunctions.getLivestockCount(function(error, count) {

				try {
					
					if(error) {
						throw(error);
					} else {

						responseArray.status = 200;
						responseArray.action = request_name;
						responseArray.data = count;
						responseArray.error = '';

						console.log(" -> getLivestockCount response: 200 - " + count);

						res.end(JSON.stringify(responseArray));

					}

				} catch(ex) {
					console.log(ex);
				}

			});

		} else if (request_name === 'getlivestock' && request_data !== '') {

			if(typeof livestockFunctions.getLivestock === "undefined") {
				throw(" > undefined function");
			} else {
				isRequest = true;
			}
			
			livestockFunctions.getLivestock(parseInt(request_data, 10), function(status, data) {

				try {
					
					responseArray.status = status;
					responseArray.action = request_name;
					responseArray.value = request_data;
					responseArray.data = data;

					if(status === 200) {
						responseArray.error = '';
					}

					console.log(" -> getLivestock response: " + status + " - " + data.data.id);

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