<?php
$speed = $_POST["v"];

include("config.php");

$sql = "update control set speed = '$speed' where stt = 0";
mysqli_query($conn,$sql);

mysqli_close($conn);
?>
