<?php

global $_connection;
$_connection = new mysqli(
	"localhost", 
	"root", 
	"", 
	"riot_smartfarm"
);

/* check connection */
if ($_connection->connect_errno) {
    die(printf("Connect failed: %s\n", $_connection->connect_error));
}

/* functions */
function escapeString($string) {
	
	if(is_null($string)) {
		return '';
	}
	
	$returnValue = $string;
	
	$returnValue = trim($returnValue);
	$returnValue = htmlspecialchars($returnValue);
	$returnValue = addslashes($returnValue);
	
	return $returnValue;
	
}

function getFenceState($fence_id) {
	
	if(is_null($fence_id)) {
		return 0;
	}
	
	if(!is_numeric($fence_id) || ($fence_id < 1)) {
		return 0;
	}
	
	global $_connection;
	
	$get = mysqli_fetch_array(
		mysqli_query(
			$_connection,
			"SELECT `state` FROM `smart_fences`
				WHERE fence_id = " . $fence_id
		)
	);
	
	return $get['state'];
	
}