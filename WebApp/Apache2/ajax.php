<?php

header("Content-type: application/json");

include(__DIR__ . '/php/database.php');

$returnArray = array(
	'status'	=> FALSE,
	'code'		=> 500,
	'notify'	=> array(
		'title'		=> 'Error',
		'message' 	=> '',
		'type'		=> 'danger'
	),
	'data'		=> ''
);

try {
	
	$getSite = (isset($_GET['site'])) ?
		escapeString($_GET['site']) : '';
	if(empty($getSite)) {
		throw new \Exception("unknown site, cannot request (1)");
	}
	
	$getAction = (isset($_GET['action'])) ?
		escapeString($_GET['action']) : '';
	if(empty($getAction)) {
		throw new \Exception("unknown action, cannot request (1)");
	}
	
	if(!file_exists('./php/ajax/' . $getSite . '.php')) {
		throw new \Exception("cannot find function: " . $getSite);
	}
	
	$returnArray['notify']['title'] = $getSite;
	
	if($getSite == 'fences') {
		
		if($getAction == 'getAll') {
				
			include('./php/ajax/' . $getSite . '.php');

			if(!isset($respondArray) || !is_array($respondArray)) {
				throw new \Exception("json_response is missing");
			}

			$returnArray['status'] = TRUE;
			$returnArray['data'] = $respondArray;

			
		} else if($getAction == 'toggleState') {
			
			$fence_id = (isset($_GET['id']) && is_numeric($_GET['id'])) ?
				(int)$_GET['id'] : 0;
			
			include('./php/ajax/' . $getSite . '.php');

			if(!isset($respondArray) || !is_array($respondArray)) {
				throw new \Exception("json_response is missing");
			}

			$returnArray['status'] = TRUE;
			$returnArray['data'] = $respondArray;
			
		} else {
			$returnArray['notify']['message'] = 'unknown action, cannot request (2)';
		}
		
	} else if($getSite == 'livestocks') {
			
		if($getAction == 'getLivestocks') {
				
			$fence_id = (isset($_GET['id']) && is_numeric($_GET['id'])) ?
				(int)$_GET['id'] : 0;

			if($fence_id < 1) {
				throw new \Exception("unknown fence id");
			}

			include('./php/ajax/' . $getSite . '.php');

			$returnArray['status'] = TRUE;
			$returnArray['data'] = $respondArray;

		} else {
			$returnArray['notify']['message'] = 'unknown action, cannot request (2)';
		}
		
	} else if($getSite == 'gates') {
			
		if($getAction == 'getGates') {
				
			$fence_id = (isset($_GET['id']) && is_numeric($_GET['id'])) ?
				(int)$_GET['id'] : 0;

			if($fence_id < 1) {
				throw new \Exception("unknown fence id");
			}

			include('./php/ajax/' . $getSite . '.php');

			$returnArray['status'] = TRUE;
			$returnArray['data'] = $respondArray;

		} else {
			$returnArray['notify']['message'] = 'unknown action, cannot request (2)';
		}
		
	} else if($getAction == 'users') {
			
		include('./php/ajax/' . $getSite . '.php');

		$returnArray['status'] = TRUE;

	} else {
		$returnArray['notify']['message'] = 'unknown site, cannot request (2)';
	}

} catch(Exception $e) {
	$returnArray['code'] = $e->getCode();
	$returnArray['notify']['message'] = $e->getMessage();
}

if(!$returnArray['status']) {
	$returnArray['notify']['title'] = 'AJAX request failed.';
} else {
	$returnArray['code'] = 200;
	$returnArray['notify']['type'] = 'success';
}

echo json_encode($returnArray);
