<?php
session_start();
if(isset($_SESSION['username'])) {
    require "../2130 - HTML/headerlogin.html";
    require "../2130 - HTML/GUI.html";
    echo "<p>Current User: " . $_SESSION['username'] . "</p>";
    echo "<p>Copyright &copy Giordan Pellegrino & Garnet Koebel</p>";

} else {
    require "../2130 - HTML/header.html";
    echo "<p>You are not authorized!!! Go away!!!!!</p>";
    
}

?>

