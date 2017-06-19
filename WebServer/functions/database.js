// MySQL
var mysql = require('mysql');

// Datenbank Anbindung
var connection = mysql.createConnection({
	host     : 'localhost',
	user     : 'root',
	password : '',
	database : 'riot_smartfarm',
	connectionLimit : 100
});

// Export
exports.connection = connection;