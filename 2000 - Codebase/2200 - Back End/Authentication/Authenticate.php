<?php
session_start();
$authenticate = False;

    // Read the JSON file 
    $au_json = file_get_contents('authorizedUsers.json');
  
    // Decode the JSON file
    $json_data = json_decode($au_json, true);
    $i = 0;
    foreach($json_data as $key => $value) {
        if($json_data[$i]['username'] === $_POST['user'] && $json_data[$i]["password"] === $_POST['pass']){
            $_SESSION['username'] = $json_data[$i]['username'];  
            require "../../2100 - Front End/2130 - HTML/headerlogin.html";
            echo "<p>Congrats, you a logged in</p>"; 
            echo "<p>Click <a href='../../2100 - Front End/2150 - PHP/member.php'> here </a> to go to the members only page</p>";
            $authenticate = true;
            
        }
        $i++;
    }
    if($authenticate == false){
        require "../../2100 - Front End/2130 - HTML/header.html";
        echo "<p>You are not authenticated!!!!</p>"; 
    }
   
?>
