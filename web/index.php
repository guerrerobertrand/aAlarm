<html>
	<head>
		<title>AAlarm monitor</title>
		<script src="jquery-1.7.2.min.js"></script>
	</head>
	<body>
		<h1>AAlarm monitor</h1>

<script>
$(document).ready(function(){
	$('#result').load('state');
});
</script>
	<div id="result">
	</div>

	<form name="adminForm" action="index.php" method="GET">
		<input type="submit" name="setOnline" value="Online"/>
		<input type="submit" name="setOffline" value="Offline"/>
	</form>
<?php

	if(isset($_GET["setOnline"]))
	{
		print "Setting to online";
		$my_file = 'command/command';
		$handle = fopen($my_file, 'w') or die('Cannot open file:  '.$my_file);
		$data = 'setOnline';
		fwrite($handle, $data);
	}
	if(isset($_GET["setOffline"]))
	{
		print "Setting to offline";
		$my_file = 'command/command';
		$handle = fopen($my_file, 'w') or die('Cannot open file:  '.$my_file);
		$data = 'setOffline';
		fwrite($handle, $data);
	}

?>
	</body>
</html>

