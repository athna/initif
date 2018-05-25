$ABS_PATH="C:\CyberRange\initif\"
$INPUT_PATH=$ABS_PATH+"initif_input_win.ps1"
$SHAPE_PATH=$ABS_PATH+"initif_shape_win.exe"
$SET_PATH=$ABS_PATH+"initif_set_win.ps1"
$CONF_PATH=$ABS_PATH+"initif.conf"
$LOG_PATH=$ABS_PATH+"log.txt"
$IPCONF_PATH=$ABS_PATH+"check_ip_addr.conf"
$eth_num=1

echo $null > $IPCONF_PATH

$CONF = (Get-Content $CONF_PATH) -as [string[]]
foreach ($line in $CONF) {
    if($line -match "^eth.*"){
        echo "setting base data:"$line >>$LOG_PATH

        $input_data=powershell $INPUT_PATH $line
        echo "Input Data: "$input_data >>$LOG_PATH
        
        $set_data=powershell  $SHAPE_PATH $input_data
        echo "set Data: "$set_data >>$LOG_PATH
        
        powershell $SET_PATH $set_data
        $eth_num=$eth_num+1
    }
}


