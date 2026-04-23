<?php
    $sock=socket_create(AF_UNIX, SOCK_DGRAM, 0);
    $cmsg=$_GET['sw'].$_GET['stat'];

    socket_sendto($sock, $cmsg, 2, 0, '/run/iot_server');
    socket_close($sock);
?>
