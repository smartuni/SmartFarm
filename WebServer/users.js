var express    = require("express");

var mysql      = require('mysql');
var connection = mysql.createConnection({
  host     : 'localhost',
  user     : 'root',
  password : '',
  database : 'riot_smartfarm'
});

var app = express();
app.set('title', 'RIOT WebServer');

const hostname = '127.0.0.1';
const port = 3000;

connection.connect(function(err){
    if(!err) {
        console.log("Database is connected ...");    
    } else {
        console.log("Error connecting database ...");    
    }
});

app.get("/count",function(req,res){
    
    connection.query('SELECT COUNT(*) AS `count` FROM smart_users', function(err, rows, fields) {
    
        if (err) 
            throw err;
            
        connection.end();

        if (!err) {
            console.log('The solution is: ', rows);
        } else {
            console.log('Error while performing Query.');
        }

        res.send(JSON.stringify(rows));

    });
});

app.get("/specific",function(req,res){
    
    connection.query(
        'SELECT * FROM smart_users WHERE user_id = ?', 
        [2],
        function(err, rows, fields) {
    
            if (!err) {

                connection.end();

                res.send(JSON.stringify(rows));

            } else {
                res.send('status=500');
            }
        }
    );

});

app.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});
