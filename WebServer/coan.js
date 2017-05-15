// CoAP
var coap = require('coap')

// Server
var server = coap.createServer(function(req, res) {
	
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

	responseArray = {
		status 	: 500,
		action 	: 'unknown',
		error	: 'unknown action'
	}
	
	// Request Titel
	var urlArray = req.url.split('/');
	
	var request_name = '';
	if(urlArray.length > 1) {
		
		request_name = urlArray[1];
		
		// Debug Output
		process.stdout.write('> action detected: ' + request_name + '\n')

	}
	
	var request_data = '';
	if(urlArray.length > 2) {
		
		request_data = urlArray[2];
		
		// Debug Output
		//process.stdout.write('> ' + request_data + '\n')
		
	}

	// Payload: Input vom Clienten
	//process.stdout.write(' -> data: ' + req.payload + '\n')

	// Requests
	if (request_name == 'getusers') {
		
		getUserCount(function(status, count) {
			
			responseArray.status = status;
			responseArray.action = request_name;
			responseArray.data = count;
			
			if(status == 200) {
				responseArray.error = '';
			}
			
			process.stdout.write(' -> getUserCount response: ' + status + ' - ' + count + '\n')
			
  			res.end(JSON.stringify(responseArray));
			
		});
		
	} if (request_name == 'getuser' && request_data != '') {
	
		getUser(parseInt(request_data, 10), function(status, data) {
			
			responseArray.status = status;
			responseArray.action = request_name;
			responseArray.value = request_data;
			responseArray.data = data;
			
			if(status == 200) {
				responseArray.error = '';
			}
			
			process.stdout.write(' -> getUser response: ' + status + ' - ' + data + '\n')
			
  			res.end(JSON.stringify(responseArray));
			
		});
				
	} else {
		// Rückgabe des JSON-Strings
		res.end(JSON.stringify(responseArray));
	}
		

})

function getUserCount(callback) {
			
	connection.query('SELECT COUNT(*) AS `count` FROM smart_users', function(err, rows) {
          
        if (!err) {
			callback(200, rows[0].count);
        } else {
			callback(500, 'query failed');
        }

    });
	
}

function getUser(user_id, callback) {
			
	if(user_id < 1) {
		callback(200, 'unknown user id');
	}
	
	connection.query('SELECT username FROM smart_users WHERE user_id = ' + user_id, function(err, rows) {
          
        if (!err) {
			if(rows.length < 1) {
				callback(404, 'user does not exist');
			} else {
				callback(200, rows[0].username);
			}
        } else {
			callback(500, 'query failed');
        }

    });
	
}

// Listener
server.listen(function () {

  process.stdout.write('Server is running..\n');
  process.stdout.write('Listening on Port 5683.\n\n');

})