<?php
$conn=new mysqli('127.0.0.1','root','wtfms233');
if(!$conn){
    die("mdzz");
}
$conn->query("SET NAMES UTF8");
$conn->select_db('gameboy')or die("cannot find");

$result_x = $conn->query('SELECT x FROM gameboy ORDER BY id DESC LIMIT 0,233');
$result_x_arr = array();   //存储数组

while($rows=mysqli_fetch_array($result_x)){
    settype($rows['x'],'int');//int
    if($rows['x']>0) $result_x_arr[] =$rows['x'];        //提取赋值
}
$result_x_arr = reverse($result_x_arr);
//转化为json格式

//取之前1小时湿度
$result_y = $conn->query('SELECT y FROM gameboy ORDER BY id DESC LIMIT 0,233');
$result_y_arr = array();   //存储数组.
while($rows=mysqli_fetch_array($result_y)){      //$rows是数组
    settype($rows['y'],'int');          //string变成int
    if($rows['y']>0)  $result_y_arr[] =$rows['y'];        //提取赋值
}
$result_y_arr = reverse($result_y_arr);
//转化为json格式
//取之前1小时光照

$json_x = json_encode($result_x_arr);
$json_y = json_encode($result_y_arr);

function reverse($array){
    $size = count($array);

    for($i=0;$i<=floor($size/2);$i++){
        $b = $array[$size-$i-1];
        $array[$size-$i-1] = $array[$i];
        $array[$i] = $b;
    }
    return $array;

}
?>


<!DOCTYPE html>
<html style="height: 100%">
<head>
    <meta charset="utf-8">

</head>
<body style="height: 100%; margin: 0">
  <a  href="clear.php">重开一局</a>
<div id="container_1" style="height: 80%"></div>
<h3 style="color:#404a59;text-align:center;" id="win">Loading。。。</h3>

<script type="text/javascript" src="http://echarts.baidu.com/gallery/vendors/echarts/echarts-all-3.js"></script>
<script type="text/javascript" src="http://echarts.baidu.com/gallery/vendors/echarts/extension/dataTool.min.js"></script>
<script type="text/javascript" src="http://echarts.baidu.com/gallery/vendors/echarts/map/js/china.js"></script>
<script type="text/javascript" src="http://echarts.baidu.com/gallery/vendors/echarts/map/js/world.js"></script>
<script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak=ZUONbpqGBsYGXNIYHicvbAbM"></script>
<script type="text/javascript" src="http://echarts.baidu.com/gallery/vendors/echarts/extension/bmap.min.js"></script>
<script type="text/javascript">

    var myChart_1 = echarts.init(document.getElementById('container_1'));
    var data1 =<?=$json_x?>;
    var data2 =<?=$json_y?>;

    var xAxisData=[];
    var win1=0;
    var win2=0;
    for (var i = 0; i < 100; i++) {
        xAxisData.push('赛程' + (i+1));
        if(data1[i]>data2[i]&&data1[i]&&data2[i]) win1++;
        if(data2[i]>data1[i]&&data2[i]&&data1[i]) win2++;

        //data1.push((Math.sin(i / 5) * (i / 5 -10) + i / 6) * 5);
       // data2.push((Math.cos(i / 5) * (i / 5 -10) + i / 6) * 5);
    }

    option = {
        title: {
            text: '战况',
    subtext: 'supported by Kaionji',
            sublink: 'http://139.199.29.58',
            x:'center',
            textStyle: {
                color: '#404a59'

            }
        },
        legend: {
            data: ['红', '蓝'],
            align: 'left'
        },
        toolbox: {
            // y: 'bottom',
            feature: {
                magicType: {
                    type: ['stack', 'tiled']
                },
                dataView: {},
                saveAsImage: {
                    pixelRatio: 2
                }
            }
        },
        tooltip: {},
        xAxis: {
            data: xAxisData,
            silent: false,
            splitLine: {
                show: false
            }
        },
        yAxis: {
        },
        series: [{
            name: 'bar',
            type: 'bar',
            data: data1,
            animationDelay: function (idx) {
                return idx * 10;
            }
        }, {
            name: 'bar2',
            type: 'bar',
            data: data2,
            animationDelay: function (idx) {
                return idx * 10 + 100;
            }
        }],
        animationEasing: 'elasticOut',
        animationDelayUpdate: function (idx) {
            return idx * 5;
        }
    };
    function myrefresh()
    {
        window.location.reload();
    }
    setTimeout('myrefresh()',15000); //指定15秒刷新一次

    myChart_1.setOption(option, true);
    document.getElementById("win").innerHTML = '红';
    document.getElementById("win").innerHTML += win1;
    document.getElementById("win").innerHTML += " : ";
    document.getElementById("win").innerHTML += '蓝';
    document.getElementById("win").innerHTML += win2;



    </script>

</body>
</html>
