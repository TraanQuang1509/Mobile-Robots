<?php
include("config.php");
$sql = "update control set auto ='0' where stt = 0";
mysqli_query($conn, $sql);
mysqli_close($conn);
?>
