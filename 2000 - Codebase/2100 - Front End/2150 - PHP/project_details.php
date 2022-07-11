<?php
session_start();
if(isset($_SESSION['username'])) {
    require "../2130 - HTML/headerlogin.html";
    require "../2130 - HTML/project_details.html";
    echo "<p>Copyright &copy Giordan Pellegrino & Garnet Koebel</p>";
}
else{
    require "../2130 - HTML/header.html";
    require "../2130 - HTML/project_details.html"; 
    echo "<p>Copyright &copy Giordan Pellegrino & Garnet Koebel</p>";
}
    

?>