<?php

$username = $_POST['user'];
$email = $_POST['email'];
$copy = FALSE;

$db = new PDO('mysql:host=127.0.0.1;dbname=elevator', 'ese', 'ese');
$db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);

$query = 'INSERT INTO authorizedusers (firstName, lastName, username, password, email, DOB) 
		  VALUES (:firstName,:lastName,:username,:password,:email,:DOB)';

$statement = $db->prepare($query);
$params = [
	'firstName' => $_POST['firstName'],
	'lastName' => $_POST['lastName'],
	'username' => $_POST['user'],
	'password' => $_POST["pass"],
	'email' =>$_POST['email'],
	'DOB' => $_POST['DOB'],
];

$query = "SELECT * FROM authorizedusers WHERE username = '$username' AND email = '$email'";
$rows = $db->query($query);
foreach ($rows as $row) {
    if($username === $row['username'] && $email === $row['email']) {
        $copy = TRUE;
    }
}

if($copy) {
	require "../../2100 - Front End/2130 - HTML/header.html";
	echo "<p>Sorry that Username or Email is already in use. please <a href='../../2100 - Front End/2150 - PHP/request_access.php'> try again. </a></p>"; 
} else {
	$results = $statement->execute($params);
	require "../../2100 - Front End/2130 - HTML/index.html";
	require "../../2100 - Front End/2130 - HTML/header.html";
}
echo "<p>Copyright &copy Giordan Pellegrino & Garnet Koebel</p>";
?>