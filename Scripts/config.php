<?php
// Connect to database
$server = "localhost";
$user = "autocar"; 
$pass = "123";
$dbname = "autocar";

$conn = mysqli_connect($server,$user,$pass,$dbname);

// Check connection
if($conn === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}


?>
