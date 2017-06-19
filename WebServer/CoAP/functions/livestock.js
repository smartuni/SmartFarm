var db = require('./database')

module.exports = {

	getLivestockCount: function(data, callback) {
		"use strict";

		try {

			console.log(" > getFenceCount");

			db.connection.query(
				'SELECT COUNT(*) AS `count` FROM smart_livestock', 
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
	getLivestock: function(rfid, callback) {
		"use strict";

		if(rfid === "") {
			callback(null, {status : 405, action : 'getLivestock', error : 'unknown livestock id'});
		} else {

			db.connection.query(
				'SELECT l.*,' +
					' c.name AS `category_name`,' +
					' f.fence_id AS `fence_id`, f.name AS `fence_name`, f.state AS `fence_state`' +
					' FROM smart_livestock l' +
					' LEFT JOIN smart_livestock_category c ON l.category_id = c.category_id' +
					' LEFT JOIN smart_fences f ON l.fence_id = f.fence_id' +
					' WHERE l.rfid = \'' + rfid + '\'', 
				function(err, rows) {

					var responseArray = {
						status 	: 500,
						action 	: 'getLivestock'
					};

					if (!err) {

						if(rows.length !== 1 || rows[0].name === null) {
							responseArray.status = 404;
							responseArray.error = 'livestock (' + rfid + ' - ' + rows.length + ') does not exist';
						} else {

							responseArray.status = 200;
							responseArray.data = {
								id			: rows[0].livestock_id,
								rfid		: rfid,
								name		: rows[0].name,
								category	: {
									id		: rows[0].category_id,
									name	: rows[0].category_name	
								},
								fence		: {
									id		: rows[0].fence_id,
									name	: rows[0].fence_name,
									state	: rows[0].fence_state
								}
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