#input eth0 192.168.122.76 192.168.122.255 192.168.122.1 24


$DEV =$Args[0]
$ipaddr =$Args[1]
$broadcast=$Args[2]
$gateway=$Args[3]
$prefix=$Args[4]
$IFNAME="Local Area Connection 3"

if ($prefix -eq 8){
    $mask ="255.0.0.0"
}elseif($prefix -eq 16){
    $mask ="255.255.0.0"
}elseif($prefix -eq 24){
    $mask ="255.255.255.0"
}


netsh interface ipv4 set address name=$IFNAME source=static address=$ipaddr mask=$mask gateway=$gateway gwmetric=1

ipconfig