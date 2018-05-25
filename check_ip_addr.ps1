$ABS_PATH="C:\CyberRange\initif\"
$CONF_PATH=$ABS_PATH+"check_ip_addr.conf"
$eth_num=1

CONF = (Get-Content $CONF_PATH) -as [string[]]
foreach ($line in $CONF) {
    
}
