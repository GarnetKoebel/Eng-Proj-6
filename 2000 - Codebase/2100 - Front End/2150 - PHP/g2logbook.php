<?php
session_start();
if(isset($_SESSION['username'])) {
    require "../2130 - HTML/headerlogin.html";
    require "../2130 - HTML/g2logbook.html";
    echo "<p>Current User: " . $_SESSION['username'] . "</p>";
    echo "<p>Copyright &copy Garnet Koebel</p>";
}
else{
    require "../2130 - HTML/header.html";
    require "../2130 - HTML/g2logbook.html"; 
    echo "<p>Copyright &copy Garnet Koebel</p>";
}

?>