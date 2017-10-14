#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
  input:
  ##################
  [DEV] [IPaddr/prefix] [Macaddr]
  ex)eth0 192.168.1.0/24 AA:BB:CC:DD:EE:FF
  ##################

  output:
  ##################
  [DEV] [IPaddr] [broadcast] [default gateway] [prefix]
  ex)eth0 192.168.1.50 192.168.1.254 192.168.1.1 24
  ##################
*/

int main( int argc, char *argv[] ){
  char *dev=argv[1];
  char *ipaddr_prefix=argv[2];
  char *macaddr=argv[3];
  char match[6];
  char *ipaddr_split_str[4];
  int ipaddr_split_int[4];
  char *macaddr_split_str[6];
  int prefix=0;
  char ipaddr[16];
  char broadcast[16];
  char gateway[16];

  for(prefix=8;prefix<=30;prefix++){
    sprintf (match,"/%d",prefix);
    if(strstr(ipaddr_prefix,match)!=NULL){
      break;
    }
  }
  if (prefix==31){
    printf("error:");
  }

  ipaddr_split_str[0]=strtok(ipaddr_prefix,".");
  ipaddr_split_int[0]=atoi(ipaddr_split_str[0]);
  ipaddr_split_str[1]=strtok(NULL,".");
  ipaddr_split_int[1]=atoi(ipaddr_split_str[1]);
  ipaddr_split_str[2]=strtok(NULL,".");
  ipaddr_split_int[2]=atoi(ipaddr_split_str[2]);
  ipaddr_split_str[3]=strtok(NULL,"/");
  ipaddr_split_int[3]=atoi(ipaddr_split_str[3]);
  
  macaddr_split_str[0]=strtok(macaddr,":");
  macaddr_split_str[1]=strtok(NULL,":");
  macaddr_split_str[2]=strtok(NULL,":");
  macaddr_split_str[3]=strtok(NULL,":");
  macaddr_split_str[4]=strtok(NULL,":");
  macaddr_split_str[5]=strtok(NULL,":");
  
  if(prefix==8){
    sprintf(ipaddr,"%s.%ld.%ld.%ld",ipaddr_split_str[0],strtol(macaddr_split_str[3],NULL,16),strtol(macaddr_split_str[4],NULL,16),strtol(macaddr_split_str[5],NULL,16));
    sprintf(broadcast,"%s.%ld.%ld.255",ipaddr_split_str[0],strtol(macaddr_split_str[3],NULL,16),strtol(macaddr_split_str[4],NULL,16));
    sprintf(gateway,"%s.%ld.%ld.1",ipaddr_split_str[0],strtol(macaddr_split_str[3],NULL,16),strtol(macaddr_split_str[4],NULL,16));
  }else if(prefix==16){
    sprintf(ipaddr,"%s.%s.%ld.%ld",ipaddr_split_str[0],ipaddr_split_str[1],strtol(macaddr_split_str[4],NULL,16),strtol(macaddr_split_str[5],NULL,16));
    sprintf(broadcast,"%s.%s.%ld.255",ipaddr_split_str[0],ipaddr_split_str[1],strtol(macaddr_split_str[4],NULL,16));
    sprintf(gateway,"%s.%s.%ld.1",ipaddr_split_str[0],ipaddr_split_str[1],strtol(macaddr_split_str[4],NULL,16));
  }else if(prefix==24){
    sprintf(ipaddr,"%s.%s.%s.%ld",ipaddr_split_str[0],ipaddr_split_str[1],ipaddr_split_str[2],strtol(macaddr_split_str[5],NULL,16));
    sprintf(broadcast,"%s.%s.%s.255",ipaddr_split_str[0],ipaddr_split_str[1],ipaddr_split_str[2]);
    sprintf(gateway,"%s.%s.%s.1",ipaddr_split_str[0],ipaddr_split_str[1],ipaddr_split_str[2]);
  }else{
    printf("Sorry, subnet mask is not supported");
  }

  /*
  puts("start initial_shape.c");
  puts("This program arrange the shape of input data");
  printf("dev is '%s'\n",dev);
  printf("IPaddr is '%s'\n",ipaddr);
  printf("broadcast is '%s'\n",broadcast);
  printf("gateway is '%s'\n",gateway);
  printf("prefix is '%d'\n",prefix);
  */
  printf("%s %s %s %s %d",dev,ipaddr,broadcast,gateway,prefix);
  return (0);
}
