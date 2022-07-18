<?php 
    // Authenticate.php
    session_start();
    $username = $_POST['user'];
    $password = $_POST['pass'];
    $authenticated = FALSE;

    $db = new PDO('mysql:host=127.0.0.1;dbname=elevator', 'ese', 'ese');
    $db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);

    // Authenticate against the database
    $query = "SELECT * FROM authorizedusers WHERE username = '$username' AND password = '$password'";
    $rows = $db->query($query);
    foreach ($rows as $row) {
        echo $row['username'];
        if($username === $row['username'] && $password === $row['password']) {
            $authenticated = TRUE;
        }
    }

    if($authenticated) {
        require "../../2100 - Front End/2130 - HTML/headerlogin.html";
        $_SESSION['username'] = $username; 
        $_SESSION['password'] = $password;  
        echo "<p>Congrats, you a logged in</p>"; 
        echo "<p>Click <a href='../../2100 - Front End/2130 - PHP/member.php'> here </a> to go to the members only page</p>";
    } else {
        require "../../2100 - Front End/2130 - HTML/header.html";
        echo "<p>You are not authenticated!!!!</p>"; 
    }
    echo "<p>Copyright &copy Giordan Pellegrino & Garnet Koebel</p>";
