<?php
        $ch=curl_init();

        curl_setopt($ch, CURLOPT_URL, "https://data.moenv.gov.tw/api/v2/aqx_p_432?api_key=4c89a32a-a214-461b-bf29-30ff32a61a8a&limit=1000&sort=ImportDate%20desc&format=JSON");
        curl_setopt($ch, CURLOPT_HEADER, false);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);


        $str=curl_exec($ch);
        //echo $str;

        $json=json_decode($str);
        //var_dump($json);

        $i=$_GET['site'];
        echo $json[$i]->{'county'}."<br>";
        echo $json[$i]->{'sitename'}."<br>";
        echo "AQI:".$json[$i]->{'aqi'}."<br>";
?>
