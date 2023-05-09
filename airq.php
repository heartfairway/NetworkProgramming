<?php
        $ch=curl_init();

        curl_setopt($ch, CURLOPT_URL, "https://data.epa.gov.tw/api/v2/aqx_p_432?api_key=e8dd42e6-9b8b-43f8-991e-b3dee723a52d&limit=1000&sort=ImportDate%20desc&format=JSON");
        curl_setopt($ch, CURLOPT_HEADER, false);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);


        $str=curl_exec($ch);
        //echo $str;

        $json=json_decode($str);
        //var_dump($json);

        $i=$_GET['site'];
        echo $json->{'records'}[$i]->{'county'}."<br>";
        echo $json->{'records'}[$i]->{'sitename'}."<br>";
        echo "AQI:".$json->{'records'}[$i]->{'aqi'}."<br>";
?>
