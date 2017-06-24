<?php

if(!isset($getAction)) {
	throw new \Exception("unknown action");
}

$respondArray = array();

if($getAction == 'getAll') {

	$query = mysqli_query(
		$_connection,
		"SELECT f.* FROM `smart_fences` f
			ORDER BY updated DESC"
	);
	if(mysqli_num_rows($query) > 0) {

		$respondArray = array();

		while($get = mysqli_fetch_array($query)) {

			$fence_id = $get['fence_id'];

			$subget = mysqli_fetch_array(
				mysqli_query(
					$_connection,
					"SELECT COUNT(livestock_id) AS `count` FROM `smart_livestock`
						WHERE fence_id = " . $fence_id
				)
			);

			$html = '';

			$stateText = ($get['state']) ? 
				'OK' : 'NOT OK';

			/* Information */
			$html .= '<p>State: <span class="pull-right">' . $stateText . '</span></p>';
			$html .= '<p>Livestock Count: <span class="pull-right">' . $subget['count'] . '</span></p>';

			/* Buttons */
			$html .= '<br /><div class="pull-right">';
			$html .= ' <button type="button" id="toggle-fence-' . $fence_id . '" class="btn btn-default btn-xs" onclick="toggleFenceState(' . $fence_id . ');">Toggle State</button>';
			$html .= ' <button type="button" id="fence-' . $fence_id . '" class="btn btn-info btn-xs" onclick="setFence(' . $fence_id . ');">Select Fence</button>';
			$html .= '</div>';

			$respondArray[] = array(
				'id' 		=> $fence_id,
				'name' 		=> $get['name'],
				'state' 	=> $get['state'],
				'updated' 	=> $get['updated'],
				'livestock' => $subget['count'],
				'div'		=> array(
					'id'	=> 'fence-container-' . $fence_id,
					'class'	=> 'fence-list-item'
				),
				'html'		=> $html
			);

		}

	}
	
} else if($getAction == 'toggleState') {
	
	if(!isset($fence_id)) {
		throw new \Exception("unknown fence id (1)");
	}
	
	if(!is_numeric($fence_id) || ($fence_id < 1)) {
		throw new \Exception("unknown fence id (2)");
	}
	
	$fence_state = getFenceState($fence_id);
	
	$new_state = ($fence_state) ? 0 : 1;
	
	$updateQuery = mysqli_query(
		$_connection,
		"UPDATE `smart_fences`
			SET state = " . $new_state . "
			WHERE fence_id = " . $fence_id
	);
		
}

