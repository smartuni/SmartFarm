// CoAP
var coap = require('coap')

// Server
var server = coap.createServer(function(req, res) {
	"use strict";
	
	res.setHeader('Content-Type', 'application/json');
	
});

// Datenbank Anbindung
var mysql      = require('mysql');
var connection = mysql.createConnection({
	host     : 'localhost',
	user     : 'root',
	password : '',
	database : 'riot_smartfarm',
	connectionLimit : 100
});

// Antwort JSON-Objekt
var responseArray;

// Handler
server.on('request', function (req, res) {
	"use strict";
	
	try {
		
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

			getUserCount(function(error, count) {

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

			getUser(parseInt(request_data, 10), function(status, data) {

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

		}

	} catch (ex) {
		console.log(ex);
	}
	
});

function getUserCount(callback) {
	"use strict";
	
	try {
		
		console.log(" > getUserCount");

		connection.query(
			'SELECT COUNT(*) AS `count` FROM smart_users', 
			function(err, rows) {

				if (err) {
					throw(err);
				} 
				
				callback(null, rows[0].count);

			}
		);
		
	} catch(ex) {
		callback(ex);
	}
	
}

function getUser(user_id, callback) {
	"use strict";
			
	if(user_id < 1) {
		callback(200, 'unknown user id');
	}
	
	connection.query(
		'SELECT username FROM smart_users WHERE user_id = ' + user_id, 
		function(err, rows) {
          
			if (!err) {
				if(rows.length < 1) {
					callback(404, 'user does not exist');
				} else {
					callback(200, rows[0].username);
				}
			} else {
				callback(500, 'query failed');
			}

		}
	);
	
}

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