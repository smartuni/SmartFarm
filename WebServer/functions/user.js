var db = require('./database')

module.exports = {
	
  getUserCount: function(callback) {
	"use strict";

	try {

		console.log(" > getUserCount");

		db.connection.query(
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

	},
	getUser: function(user_id, callback) {
		"use strict";

		if(user_id < 1) {
			callback(200, 'unknown user id');
		}

		db.connection.query(
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
	
};



