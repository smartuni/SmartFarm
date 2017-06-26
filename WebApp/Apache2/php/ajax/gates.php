<?php

$respondArray = array();

if(!isset($_connection)) {
	throw new \Exception("unknown database connection");
}

if(!isset($getAction)) {
	throw new \Exception("unknown action");
}

if($getAction == 'getGates') {

	if(!isset($fence_id)) {
		throw new \Exception("unknown fence id (1)");
	}
	
	if(!is_numeric($fence_id) || ($fence_id < 1)) {
		throw new \Exception("unknown fence id (2)");
	}

	
	$query = mysqli_query(
		$_connection,
		"SELECT g.* FROM `smart_fences_gates` g
			WHERE fence_id = " . $fence_id . "
			ORDER BY updated DESC"
	);
	if(mysqli_num_rows($query) > 0) {

		$respondArray = array();

		while($get = mysqli_fetch_array($query)) {

			$gate_id = $get['gate_id'];

			$html = '';

			$stateText = ($get['state']) ? 
				'OK' : 'NOT OK';

			/* Information */
			$html .= '<p>State: <span class="pull-right">' . $stateText . '</span></p>';
			
			/* Buttons */
			$html .= '<br /><div class="pull-right">';
			$html .= ' <button type="button" id="toggle-gate-' . $gate_id . '" class="btn btn-default btn-xs" onclick="toggleGateState(' . $gate_id . ');">Toggle State</button>';
			$html .= '</div>';

			$respondArray[] = array(
				'id' 		=> $gate_id,
				'name' 		=> $get['name'],
				'state' 	=> $get['state'],
				'updated' 	=> $get['updated'],
				'div'		=> array(
					'id'	=> 'gate-container-' . $fence_id,
					'class'	=> 'gate-list-item'
				),
				'html'		=> $html
			);

		}

	}
	
}
