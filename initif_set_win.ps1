#input eth0 192.168.122.76 192.168.122.255 192.168.122.1 24
$ABS_PATH="C:\CyberRange\initif\"
$LOG_PATH=$ABS_PATH+"log.txt"

$Idx =$Args[0]
$ipaddr =$Args[1]
$broadcast=$Args[2]
$gateway=$Args[3]
$prefix=$Args[4]

if ($prefix -eq 8){
    $mask ="255.0.0.0"
}elseif($prefix -eq 16){
    $mask ="255.255.0.0"
}elseif($prefix -eq 24){
    $mask ="255.255.255.0"
}


netsh interface ipv4 set address name=$Idx source=static address=$ipaddr mask=$mask gateway=$gateway gwmetric=1
netsh interface ip set dns name=$Idx static $gateway primary