var db = require('./database')

module.exports = {

	getFenceCount: function(data, callback) {
		"use strict";

		try {

		console.log(" > getFenceCount");

		db.connection.query(
			'SELECT COUNT(*) AS `count` FROM smart_fences', 
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
	getFence: function(data, callback) {
		"use strict";

		var fence_id = (typeof data.id !== "undefined") ?
			data.id : 0;

		if(fence_id < 1) {
			callback({status : 500, action : 'getFence', error : 'unknown fence id'});
		} else {

			db.connection.query(
				'SELECT f.*, COUNT(g.gate_id) AS `gate_count` FROM smart_fences f' +
					' JOIN smart_fences_gates g ON f.fence_id = g.fence_id' +
					' WHERE f.fence_id = ' + fence_id, 
				function(err, rows) {

					var responseArray = {
						status 	: 500,
						action 	: 'getFence'
					};

					if (!err) {

						if(rows.length !== 1 || rows[0].name === null) {
							responseArray.status = 404;
							responseArray.error = 'fence does not exist';
						} else {

							responseArray.status = 200;
							responseArray.data = {
								id			: rows[0].fence_id,
								name		: rows[0].name,
								state		: rows[0].state,
								updated		: rows[0].updated,
								gate_count	: rows[0].gate_count
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

	},
	setFenceState: function(data, callback) {

		"use strict";

		if(typeof data.id === "undefined" || (data.id < 1)) {
			callback({status : 401, action : 'setFenceState', error : 'unknown fence id'});
		} else {

			if(typeof data.state === "undefined" || (data.state < 0)) {
				callback({status : 402, action : 'setFenceState', error : 'unknown fence state'});
			} else {
					
				db.connection.query(
					'UPDATE smart_fences' +
						' SET state = ' + data.state +
						' WHERE fence_id = ' + data.id, 
					function(err, rows) {

						var responseArray = {
							status 	: 500,
							action 	: 'setFenceState'
						};

						if (!err) {

							responseArray.status = 200;
							responseArray.data = {
								id			: data.id,
								state		: data.state
							};

						} else {
							responseArray.status = 500;
							responseArray.error = 'query failed (' + err.message + ')';
						}

						callback(null, responseArray);

					}

				);
			
			}

		}

	}
	
};