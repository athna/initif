$ABS_PATH="\\RASPBERRYPI\share\initif\"
$INPUT_PATH="windows_initif_input.ps1"
$INPUT_TXT_PATH="initif_input.txt"
$SHAPE_PATH="initif_shape.exe"
$SET_PATH="windows_initif_set.ps1"
$CONF_PATH="initif.conf"
$eth_num=1

#compile
#gcc -Wall -o $SHAPE_PATH initif_shape.c

$CONF = (Get-Content $ABS_PATH$CONF_PATH) -as [string[]]
foreach ($line in $CONF) {
    if($line -match "^eth.*"){
        $input_data=powershell .\$INPUT_PATH $line $eth_num
        $set_data=powershell .\$SHAPE_PATH $input_data
        #echo $set_data
        #powershell .\$SET_PATH $set_data
        $eth_num=$eth_num+1
    }
}


