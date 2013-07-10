<?php
	require 'conf.php';

	/** Error codes. These will be displayed by the server in case of errors **/
	define('ERROR_INVALID_VERSION', 'This masterlist does not offer support for version %s');
	define('ERROR_INVALID_PORT', 'Invalid server port');
	define('ERROR_BANNED', 'You have been banned. DATE: %s REASON: %s');
	define('ERROR_DB_CONNECTION', 'Database connection failed');
	define('ERROR_DB_INSERT', 'Failed to add server to database');
	define('ADDED', 'Added'); //Server successfully added code


	$address 	= $_SERVER['REMOTE_ADDR'];
	$version 	= $_REQUEST['version'];
	$port		= $_REQUEST['port'];

	/** Port range check **/
	if(!is_numeric($port) || $port < 1024 || $port > 65535){
		exit(ERROR_INVALID_PORT);
	}

	/** Version check **/
	if(!in_array($version, $supported_versions)){
		exit(sprintf(ERROR_INVALID_VERSION, $version));
	}

	/** Database connection **/
	$db = new mysqli(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD, MYSQL_TABLE);
	
	if($db->connect_errno){
		exit(ERROR_DB_CONNECTION);
	}

	/** Checking if ip was banned **/
	if($query_bans = $db->query("SELECT ban_date, reason FROM bans WHERE address='{$address}'")){
		$banned = $query_bans->num_rows;
		$reason = '';
		$date = '';
		if($banned){
			$row = $query_bans->fetch_assoc();
			/** Format error message with ban date and reason **/
			echo sprintf(ERROR_BANNED, $row['ban_date'], $row['reason']);
		}
		$query_bans->close();

		if($banned){
			$db->close();
			exit;
		}
	}

	/** Tries to update server's last post **/
	$db->query("UPDATE servers SET last_post=NOW(), version='{$version}' WHERE address='{$address}' AND port='{$port}'");
	/** If the server is not on the database, insert it **/
	if(!$db->affected_rows){
		$db->query("INSERT INTO servers (address, port, version, last_post) VALUES ('{$address}', '{$port}', '{$version}', NOW())");
	}

	/** If rows where affected, the server was added (or updated) **/
	if($db->affected_rows){
		echo ADDED;
	}else{
		echo ERROR_DB_INSERT;
	}
	$db->close();
	exit;

?>