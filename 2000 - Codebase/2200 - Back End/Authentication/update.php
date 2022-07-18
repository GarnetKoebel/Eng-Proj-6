<?php 

function update_elevatorNetwork(int $node_ID, int $new_status) : void {
    $db = new PDO(
        'mysql:host=127.0.0.1;dbname=elevator',
        'giordan',
        ''
    );
    $db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);

    $query = 'UPDATE elevatorNetwork SET State = :stat WHERE nodeID = :id' ;
    $statement = $db->prepare($query); 
    $statement->bindValue('stat', $new_status); 
    $statement->bindValue('id', $node_ID); 
    $statement->execute();                      // Execute prepared statement
}

session_start();
if(isset($_SESSION['username'])) {
    require "../../2100 - Front End/2130 - HTML/headerlogin.html";
    require "../../2100 - Front End/2130 - HTML/GUI.html";

    $db = new PDO(
        'mysql:host=127.0.0.1;dbname=elevator',
        'giordan',
        ''
    );
    $db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);

    if ($_POST['action'] == "Floor 3") {
        update_elevatorNetwork(4, 3);
        echo "<script>

        var audio = new Audio('../../2100 - Front End/2170 - Videos/Floor3.mp3');
        audio.play();

        </script>";

    } else if ($_POST['action'] == "Floor 2") {
        update_elevatorNetwork(4, 2);
        echo "<script>

        var audio = new Audio('../../2100 - Front End/2170 - Videos/Floor2.mp3');
        audio.play();

        </script>";

    } else if ($_POST['action'] == "Floor 1") {
        update_elevatorNetwork(4, 1);
        echo "<script>

        var audio = new Audio('../../2100 - Front End/2170 - Videos/Floor1.mp3');
        audio.play();

        </script>";

    } else if ($_POST['action'] == "Dev") {
        $query = 'SELECT currentFloor, F1Reached, F2Reached, F3Reached FROM elevatorNetwork'; 
        $rows = $db->query($query);
        foreach ($rows as $row) {
            echo "<p> Current floor is:" . $row['currentFloor'] . " </p>";
            echo "<p> Floor 1 reached:" . $row['F1Reached'] . " Times</p>";
            echo "<p> Floor 2 reached:" . $row['F2Reached'] . " Times</p>";
            echo "<p> Floor 3 reached:" . $row['F3Reached'] . " Times</p>";
        }
    } else if ($_POST['action'] == "Sabbith") {
        update_elevatorNetwork(4, 1);
        sleep(10);
        update_elevatorNetwork(4, 2);
        sleep(10);
        update_elevatorNetwork(4, 3);
    } else {
        //invalid action!
    }
    echo "<p>Current User: " . $_SESSION['username'] . "</p>";

} else {
    require "../../2100 - Front end/2130 - HTML/header.html";
    echo "<p>You are not authorized!!! Go away!!!!!</p>";
    
}
 echo "<p>Copyright &copy Giordan Pellegrino & Garnet Koebel</p>";
?>

