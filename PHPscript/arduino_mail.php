<?php

$to = $_POST['to'];
$subject = $_POST['subject'];
$txt = $_POST['txt'];
$key = trim($_POST['key']);

$headers = "From: arduino@2noodles.com". "\r\n"; // change the domain to your domain so the mail is not spam

if($key == "greencontroller") {
	
	//mail($to,$subject,$txt,$headers);

	if(mail($to,$subject,$txt,$headers)) {
		echo "mail sent";
		echo " to: " . $to;
	} else {
		echo "can't send mail";
		echo " to: " . $to;

	}

	
} else {
	
	echo "to: " . $to;
	echo "wrong key: " . $key;	
	
}

?>