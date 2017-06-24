<?php

require( __DIR__ . '/../../vendor/autoload.php' );

$loop = React\EventLoop\Factory::create();

$client = new PhpCoap\Client\Client( $loop );

$client->get( 'coap://localhost/getUserCount', function( $data ) {
	echo '<pre>';
	print_r(json_decode( $data ));
	echo '</pre>';
} );

$loop->run();
