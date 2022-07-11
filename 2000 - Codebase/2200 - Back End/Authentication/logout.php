<?php 
    session_start(); 
    session_destroy(); 

    require '../../2100 - Front End/2130 - HTML/header.html';

    echo "<h1>You have been logged out successfully</h1>";
    echo "<p>Please click <a href='../../2100 - Front End/2150 - PHP/login.php'>here</a> to log in again</p>";
    echo "<p>Please click <a href='../../2100 - Front End/2150 - PHP/index.php'>here</a> to return to main menu</p>";

?>