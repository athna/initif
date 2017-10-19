$DEV =$Args[0]
$IPaddr =$Args[1]
$eth_num=$Args[2]


$GETMAC=getmac /fo csv
$MACaddr=$GETMAC[$eth_num][1]+$GETMAC[$eth_num][2]+":"+$GETMAC[$eth_num][4]+$GETMAC[$eth_num][5]+":"+$GETMAC[$eth_num][7]+$GETMAC[$eth_num][8]+":"+$GETMAC[$eth_num][10]+$GETMAC[$eth_num][11]+":"+$GETMAC[$eth_num][13]+$GETMAC[$eth_num][14]+":"+$GETMAC[$eth_num][16]+$GETMAC[$eth_num][17]


$result=$DEV+" "+$IPaddr+" "+$MACaddr

echo $result