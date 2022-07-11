<?php

if(file_exists('authorizedUsers.json'))
{
     $json_data=fileWriteAppend();
     if(file_put_contents('authorizedUsers.json', $json_data))
     {
          $message = "<label class='text-success'>Data added Success fully</p>";
     }
}
else
{
     $json_data=fileCreateWrite();
     if(file_put_contents('authorizedUsers.json', $json_data))
     {
          $message = "<label class='text-success'>File createed and  data added Success fully</p>";
     }

}
function fileWriteAppend(){
		$current_data = file_get_contents('authorizedUsers.json');
		$array_data = json_decode($current_data, true);
		$extra = array(
			 'firstName' => $_POST['firstName'],
			 'lastName' => $_POST['lastName'],
			 'username' => $_POST['user'],
			 'password' => $_POST["pass"],
			 'email' =>$_POST['email'],
			 'DOB' => $_POST['DOB'],
			 'FoS' => $_POST['fac_or_student'],
			 'driver' => $_POST['driver'],

		);
		$array_data[] = $extra;
		$json_data = json_encode($array_data, JSON_PRETTY_PRINT);
		return $json_data;
}
function fileCreateWrite(){
		$file=fopen("authorizedUsers.json","w");
		$json_data=array();
		$extra = array(
			 'firstName' => $_POST['firstName'],
			 'lastName' => $_POST['lastName'],
			 'username' => $_POST['user'],
			 'password' => $_POST["pass"],
			 'email' =>$_POST['email'],
			 'DOB' => $_POST['DOB'],
			 'FoS' => $_POST['fac_or_student'],
			 'driver' => $_POST['driver'],

		);
		$array_data[] = $extra;
		$json_data = json_encode($array_data, JSON_PRETTY_PRINT);
		fclose($file);
		return $json_data;
}

	require "../../2100 - Front End/2130 - HTML/header.html";
    echo "<p>Copyright &copy Giordan Pellegrino & Garnet Koebel</p>";

?>