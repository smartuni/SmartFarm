var db = require('./database')

module.exports = {
	
  getUserCount: function(data, callback) {
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
	getUser: function(data, callback) {
		"use strict";

		var user_id = (typeof data.id !== "undefined") ?
			data.id : 0;

		if(user_id === null || user_id < 1) {
			callback({status: 405, error: 'unknown user id'});
		} else {
			
			db.connection.query(
				'SELECT username FROM smart_users' +
					' WHERE user_id = ' + user_id, 
				function(err, rows) {

					var responseArray = {
						status	: 500,
						error	: ''
					};

					if (!err) {

						if(rows.length < 1 || rows[0].username === null) {
							responseArray.status = 404;
							responseArray.error = 'user does not exist';
						} else {
							responseArray.status = 200;
							responseArray.data = {
								name	: rows[0].username
							};
						}

					} else {
						responseArray.status = 500;
						responseArray.error = 'query failed';
					}

					callback(null, responseArray);

				}

			);

		}

	}
	
};



