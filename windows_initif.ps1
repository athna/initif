$ABS_PATH="C:\CyberRange\initif\"
$INPUT_PATH="windows_initif_input.ps1"
$INPUT_TXT_PATH="initif_input.txt"
$SHAPE_PATH="initif_shape.exe"
$SET_PATH="windows_initif_set.ps1"
$CONF_PATH="initif.conf"
$LOG_PATH=$ABS_PATH+"log.txt"
$eth_num=1

#compile
#gcc -Wall -o $SHAPE_PATH initif_shape.c

$CONF = (Get-Content $ABS_PATH$CONF_PATH) -as [string[]]
foreach ($line in $CONF) {
    if($line -match "^eth.*"){
        echo $line >>$LOG_PATH
        $PATH =$ABS_PATH + $INPUT_PATH
        $input_data=powershell $PATH $line $eth_num
        echo $input_data >>$LOG_PATH
        $PATH =$ABS_PATH + $SHAPE_PATH
        $set_data=powershell $PATH $input_data
        echo $set_data >>$LOG_PATH
        $PATH =$ABS_PATH + $SET_PATH
        powershell $PATH $set_data
        $eth_num=$eth_num+1
        #echo $eth_num
    }
}


