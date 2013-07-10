<?php
	require 'conf.php';

	/** Maximum time a server can live without posting itself again (in seconds) **/
	define("SERVER_TIMEOUT", 60);

	$address 	= $_SERVER['REMOTE_ADDR'];
	$version 	= $_REQUEST['version'];

	/** Uncomment next line to log requests **/
	//file_put_contents("access.log", "Request received from {$address} using version {$version} \r\n", FILE_APPEND);

	/** Version check **/
	if(!in_array($version, $supported_versions)){
		exit;
	}

	/** Database connection **/
	$db = new mysqli(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD, MYSQL_TABLE);
	
	if($db->connect_errno){
		exit;
	}

	/** Cleanup servers that timed out **/
	$db->query("DELETE FROM servers WHERE TIME_TO_SEC(TIMEDIFF(NOW(), last_post)) >= {SERVER_TIMEOUT}");

	if($query_servers = $db->query("SELECT * FROM servers WHERE version='{$version}'")){
		while($server = $query_servers->fetch_assoc()){
			echo sprintf("%s:%d\n", $server["address"], $server["port"]);
		}
		$query_servers->close();
	}

	$db->close();
	exit;

?>