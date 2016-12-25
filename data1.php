
<?php
$tem = $_GET["x"];

$conn = @mysql_connect('127.0.0.1', 'root', 'wtfms233');   //链接服务器
if (!$conn) {
    die("failed");
}
else print_r('success');
@mysql_query("SET NAMES UTF8");
@mysql_select_db('gameboy', $conn) or die("1024");             //选择数据库
if(empty($tem))
{
    $result= @mysql_query('SELECT * FROM gameboy ORDER BY did DESC LIMIT 0,1',$conn);
    $result_arr= @mysql_fetch_assoc($result);
    $tem_new=$result_arr['tem'];

    $insert="insert into gameboy(y) VALUES ('$tem_new')";

}
else{
    $insert="insert into gameboy(y) VALUES ('$tem')";}
@mysql_query($insert,$conn);
?>
