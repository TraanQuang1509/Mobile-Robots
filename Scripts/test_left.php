<?php
include("config.php");
$sql = "update control set up1 = '0', down1 = '0', left1 = '0', right1 = '0', motor_left = '1', motor_right = '0', auto = '0' where stt = 0";
mysqli_query($conn, $sql);
mysqli_close($conn);

?>
