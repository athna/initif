#input eth0 192.168.122.76 192.168.122.255 192.168.122.1 24
$ABS_PATH="C:\CyberRange\initif\"
$LOG_PATH=$ABS_PATH+"log.txt"

$DEV =$Args[0]
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

if ($DEV -eq "eth0"){
    $Idx=13
}
if($DEV -eq "eth1"){
    $Idx=15
}

echo $DEV $Idx $ipaddr $broadcast $gateway $prefix $mask >> $LOG_PATH
netsh interface ipv4 set address name=$Idx source=static address=$ipaddr mask=$mask gateway=$gateway gwmetric=1