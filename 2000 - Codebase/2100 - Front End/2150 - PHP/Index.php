<?php 
session_start();
if(isset($_SESSION['username'])) {
    require "../2130 - HTML/headerlogin.html";
    require "../2130 - HTML/indexlogin.html";
    echo "<p>Copyright &copy Giordan Pellegrino & Garnet Koebel</p>";
}
else{
    require "../2130 - HTML/header.html";
    require "../2130 - HTML/index.html";
    echo "<p>Copyright &copy Giordan Pellegrino & Garnet Koebel</p>";
}
    
?>
