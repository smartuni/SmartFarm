<?php

$respondArray = array();

if(!isset($_connection)) {
	throw new \Exception("unknown database connection");
}

if(!isset($fence_id)) {
	throw new \Exception("unknown fence id (1)");
}

if(!is_numeric($fence_id) || ($fence_id < 1)) {
	throw new \Exception("unknown fence id (2)");
}

$query = mysqli_query(
	$_connection,
	"SELECT 
			l.*,
			c.name AS `category_name`
		FROM `smart_livestock` l
		LEFT JOIN `smart_livestock_category` c ON l.category_id = c.category_id
		WHERE l.fence_id = " . $fence_id 
);

$anz = mysqli_num_rows($query);
if($anz > 0) {

	$respondArray = array();

	while($get = mysqli_fetch_array($query)) {

		$livestock_id = $get['livestock_id'];

		$html = '';
		$html .= '<p>RFID Tag: <span class="pull-right">' . $get['rfid'] . '</span></p>';
		$html .= '<p>Category: <span class="pull-right">' . $get['category_name'] . '</span></p>';
		
		$respondArray[] = array(
			'id' 		=> $livestock_id,
			'name' 		=> $get['name'],
			'rfid' 		=> $get['rfid'],
			'updated' 	=> $get['updated'],
			'category'	=> array(
				'id'	=> $get['category_id'],
				'name'	=> $get['category_name']
			),
			'div'		=> array(
				'id'	=> 'livestock-' . $livestock_id,
				'class'	=> 'livestock-list-item'
			),
			'html'		=> $html
		);

	}

}
