$ABS_PATH="C:\CyberRange\initif\"
$IPCONF_PATH=$ABS_PATH+"check_ip_addr.conf"
$eth_num=1

$IPCONF = (Get-Content $IPCONF_PATH) -as [string[]]
foreach ($ipaddr in $IPCONF) {
    $ipconfig=ipconfig
    for($i=1;($ipconfig[$i] -ne 0) -and ($ipconfig[$i] -ne $null);$i++){
        if(($ipconfig[$i].Contains("IPv4 Address")) -and ($ipconfig[$i].Contains($ipaddr))){
            break
        }
    }
}