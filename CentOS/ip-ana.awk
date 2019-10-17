#!awk
#
# % ip -d address | this-program
#
#
BEGIN {
    dn=""
}
/^[0-9]*:/{
    split($2,v,":");
#   dn=v[1]
    split(v[1],u,"@");
    dn=u[1]
}
#/^ +link\/loopback/ {
#   next
#}
/^ +link\// {
    dn2mac[dn]=$2
    if(mac2dn[$2] != "") {
        mac2dn[$2]=mac2dn[$2] "," dn
    } else {
        mac2dn[$2]=dn
    }
}
/^ +inet / {
    dn2ip[dn]=$2
}
/^ +vlan protocol/ {
    dn2vlan[dn]=$5
}
END {
    for(d in dn2mac) {
        if(dn2ip[d] == "")  { dn2ip[d] = "-" }
        if(dn2vlan[d] == "")    { dn2vlan[d] = "-" }
    }

    fmt="%-20s %17s %18s %4s\n"
    #printf fmt, "#dev", "mac", "ipv4", "vlan"
    for(d in dn2mac) {
        printf fmt, d,dn2mac[d],dn2ip[d],dn2vlan[d]
    }
#   for(m in mac2dn) {
#       print m,mac2dn[m]
#   }
}
