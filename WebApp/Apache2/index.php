<?php
include(__DIR__ . '/php/database.php');
?>
<!doctype html>
<html>
<head>
	<meta charset="utf-8">
	<title>RIOT SmartFarm</title>
	
	<link rel="stylesheet" href="./static/dist/css/bootstrap.min.css" />
	<link rel="stylesheet" href="./static/dist/css/bootstrap-theme.min.css" />
	<link rel="stylesheet" href="./static/dist/css/styles.min.css" />

	<script type="text/javascript" src="./static/dist/js/jquery-3.2.1.min.js"></script>
	<script type="text/javascript" src="./static/dist/js/bootstrap.min.js"></script>
	<script type="text/javascript" src="./static/dist/js/bootstrap-notify.min.js"></script>
	<script type="text/javascript" src="./static/dist/js/scripts.min.js"></script>
	
</head>

<body>
	<nav class="navbar navbar-default">
		<div class="container-fluid">
			<div class="navbar-header">
				<button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1" aria-expanded="false">
					<span class="sr-only">Toggle navigation</span>
					<span class="icon-bar"></span>
					<span class="icon-bar"></span>
					<span class="icon-bar"></span>
				</button>
				<a class="navbar-brand" href="#">SmartFarm</a>
			</div>
			<div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
				<ul class="nav navbar-nav">
					<li class="active">
						<a href="index.php">Home</a>
					</li>
				</ul>
			</div>
		</div>
	</nav>
	<div class="container">
		<div class="row">
			<div class="col-sm-6">
				<div class="panel panel-default">
					<div class="panel-heading">
						<h1 class="panel-title">Fences</h1>
					</div>
					<div id="fencesContainer">
						<div class="panel-body">
							<img src="./static/images/loader.gif" alt="Pending.." />
						</div>
					</div>
				</div>
				<script type="text/javascript">
					$(document).ready(function() {
						
						try {
							
							getAJAXData(
								'fencesContainer', 
								'ajax.php?site=fences&action=getAll',
								false
							);
							
						} catch(err) {
							console.log(err.message);
						}

					});
				</script>
			</div>
			<div class="col-sm-6">
				<div class="panel panel-default">
					<div class="panel-heading">
						<h1 class="panel-title">Livestocks</h1>
					</div>
					<div id="livestockContainer">
						<div class="panel-body">
							<span class="gray">select a fence</span>
						</div>
					</div>
				</div>
				<div class="panel panel-default">
					<div class="panel-heading">
						<h1 class="panel-title">Gates</h1>
					</div>
					<div id="gateContainer">
						<div class="panel-body">
							<span class="gray">select a fence</span>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
	<div class="container" id="resultArray" style="display: none;"></div>
</body>
</html>