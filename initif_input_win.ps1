$DEV =$Args[0]
$IPaddr =$Args[1]
$MACaddr_conf=$Args[2]
$eth_num=-999


#get mac address
getmac /fo csv /v > getmac.csv
$GETMAC=Import-Csv getmac.csv
$GETMAC_addr=$GETMAC."Physical Address"
$GETMAC_line=(Get-Content .\getmac.csv).Length #=2:one interface ,>=3:some interface

if (($GETMAC_line -eq 2) -or ($MACaddr_conf -eq "ZZ:ZZ")){
    if ($GETMAC_addr.Length -eq 1){
        $MACaddr=$GETMAC_addr[0]+$GETMAC_addr[1]+":"+$GETMAC_addr[3]+$GETMAC_addr[4]+":"+$GETMAC_addr[6]+$GETMAC_addr[7]+":"+$GETMAC_addr[9]+$GETMAC_addr[10]+":"+$GETMAC_addr[12]+$GETMAC_addr[13]+":"+$GETMAC_addr[15]+$GETMAC_addr[16]
    }else{
        for($i=0;$i -eq $GETMAC_addr.Length;$i++){
            if($GETMAC_addr[$i].Length -eq 17){
                $MACaddr=$GETMAC_addr[$i][0]+$GETMAC_addr[$i][1]+":"+$GETMAC_addr[$i][3]+$GETMAC_addr[$i][4]+":"+$GETMAC_addr[$i][6]+$GETMAC_addr[$i][7]+":"+$GETMAC_addr[$i][9]+$GETMAC_addr[$i][10]+":"+$GETMAC_addr[$i][12]+$GETMAC_addr[$i][13]+":"+$GETMAC_addr[$i][15]+$GETMAC_addr[$i][16]
            }
        }
    }
}else{
    for($i=0;$GETMAC_addr[$i] -ne 0;$i++){
        if(($GETMAC_addr[$i][12] -eq $MACaddr_conf[0])-and($GETMAC_addr[$i][13] -eq $MACaddr_conf[1])-and($GETMAC_addr[$i][15] -eq $MACaddr_conf[3])-and($GETMAC_addr[$i][16] -eq $MACaddr_conf[4])){
            $eth_num=$i
            break
        }
    }
    #TODO:when $eth_num==-999->error
    $MACaddr=$GETMAC_addr[$eth_num][0]+$GETMAC_addr[$eth_num][1]+":"+$GETMAC_addr[$eth_num][3]+$GETMAC_addr[$eth_num][4]+":"+$GETMAC_addr[$eth_num][6]+$GETMAC_addr[$eth_num][7]+":"+$GETMAC_addr[$eth_num][9]+$GETMAC_addr[$eth_num][10]+":"+$GETMAC_addr[$eth_num][12]+$GETMAC_addr[$eth_num][13]+":"+$GETMAC_addr[$eth_num][15]+$GETMAC_addr[$eth_num][16]
}

#get Connection Name
if (($GETMAC_line -eq 2) -or ($MACaddr_conf -eq "ZZ:ZZ")){
    $Con_Name=$GETMAC."Connection Name"
}else{
    $Con_Name=$GETMAC."Connection Name"[$eth_num]
}

#get index
$netsh=netsh interface ipv4 show interface
for($i=1;($netsh[$i] -ne 0) -and ($netsh[$i] -ne $null);$i++){
    for($j=0;$j -ne $Con_Name.Length;$j++){
        if($netsh[$i].Contains($Con_Name[$j])){
            $Idx= $netsh[$i][0]+$netsh[$i][1]+$netsh[$i][2]
            $Idx= [int]$Idx
            break
        }
    }
}

$result=[string]$Idx+" "+$IPaddr+" "+$MACaddr
Remove-Item getmac.csv
echo $result