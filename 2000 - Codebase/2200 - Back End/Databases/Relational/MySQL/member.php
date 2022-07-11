<?php 
    // member.php
    session_start(); 

    // members only section
    if(isset($_SESSION['username'])) {
        echo "Welcome, " . $_SESSION['username'] . "<br />";


        echo "<h1>Members only content goes here </h1>";

        // Sign out option
        echo "<p>Click <a href='logout.php'>here</a> to log out</p>";

    } else {
        echo "<p>You are not authorized!!! Go away!!!!!</p>";
    }

?>
