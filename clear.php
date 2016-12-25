<?php
$conn= @mysql_connect('127.0.0.1','root','wtfms233');
  $hhh = 'TRUNCATE TABLE gameboy';
  @mysql_select_db('gameboy');
  $mdzz= @mysql_query($hhh, $conn);
  if(!$mdzz ) die("mdzz");
else echo "ok";  ?>

<!DOCTYPE html>
<html style="height: 100%">
<head>
    <meta charset="utf-8">

</head>
<body style="height: 100%; margin: 0">
<a href="index.php">Home</a>
</body>
</html>
