<?php 

function update_elevatorNetwork(int $node_ID, int $new_status) {
    $db = new PDO('mysql:host=127.0.0.1;dbname=elevator', 'ese', 'ese');
    $db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);
    $query = 'UPDATE elevatornetwork SET State = :stat, date = current_date, time = current_time WHERE nodeID = :id';
    $statement = $db->prepare($query); 
    $statement->bindValue('stat', $new_status); 
    $statement->bindValue('id', $node_ID); 
    $statement->execute();                      // Execute prepared statement
    return 0; 
}

function display(){
    $servername = "localhost";
    $username = "ese";
    $password = "ese";
    $dbname = "elevator";

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);

    $sql = "SELECT * FROM elevatornetwork";
    $result = $conn->query($sql);

    if ($result->num_rows > 0) {
    // output data of each row
        while($row = $result->fetch_assoc()) {
            echo "<p>Date: " . $row["date"]. " - Time: " . $row["time"]. " - State: " . $row["State"]. " - nodeID: " . $row["nodeID"]. " - currentFloor: " . $row["currentFloor"]. " - F1Reached: " . $row["F1Reached"]. " - F2Reached: " . $row["F2Reached"]. " - F3Reached: " . $row["F3Reached"]."<p><br>";
        }
    } else {
        echo "0 results";
    }
    $conn->close();

}

session_start();

if(isset($_SESSION['username'])) {
    require "../../2100 - Front End/2130 - HTML/headerlogin.html";
    require "../../2100 - Front End/2130 - HTML/GUI.html";

    if ($_POST['action'] == "Floor 3") {
        update_elevatorNetwork(1, 3);

    } else if ($_POST['action'] == "Floor 2") {
        update_elevatorNetwork(1, 2);

    } else if ($_POST['action'] == "Floor 1") {
        update_elevatorNetwork(1, 1);

    } else if ($_POST['action'] == "Sabbath") {
     
    } else if ($_POST['action'] == "Update") {
        
    } else {
        echo "Invalid";
    }
    display();     
    echo "<p>Current User: " . $_SESSION['username'] . "</p>";

} else {
    require "../../2100 - Front end/2130 - HTML/header.html";
    echo "<p>You are not authorized!!! Go away!!!!!</p>";
    
}
 echo "<p>Copyright &copy Giordan Pellegrino & Garnet Koebel</p>";
?>

