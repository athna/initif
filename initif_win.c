#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#define DEBUG false

//compile command
//gcc -O3 -mtune=native -march=native  -o initif .\initif_win.c

struct MAC_index
{
    char CH_MAC_addr[6][4];
    int INT_MAC_addr[6];
    int index;
};

int get_interface_info(struct MAC_index MAC_index_list[10]);
int calc_subnet_mask(int prefix, char set_subnet_mask[16]);
int calc_index_number(char conf_MAC_addr[6][4], struct MAC_index MAC_index_list[10], int MAX_MAC_index_num);
int calc_ip_address(int net_IP_addr[4], int prefix, int MAC_addr[6], char set_IP_addr[16]);
int calc_default_gw(int net_IP_addr[4], int prefix, int MAC_addr[6], char set_default_gateway[16]);
void setup_interface(char set_index_number[4], char set_IP_addr[16], char set_subnet_mask[16], char set_default_gateway[16]);
int binary_to_decimal(char binary_number[8]);

int main(void)
{
    //char file_name[256] = "F:\\Dropbox\\Workspace\\initif_for_windows\\initif.conf";
    //char file_name[256] = "C:\\Dropbox\\Workspace\\initif_for_windows\\initif.conf";
    //char file_name[256] = "E:\\initif.conf";
    char file_name[256] = "C:\\CyberRange\\initif\\initif.conf";
    int net_IP_addr[4] = {0};
    int prefix = 0;
    char conf_MAC_addr[6][4] = {{'\0'},{'\0'}};
    int VLAN = 0;
    char config_file_row[256] = {'\0'};
    char *temp_ptr;
    struct MAC_index MAC_index_list[10];
    int MAX_MAC_index_num = 0;
    int MAC_index_num = 0;
    char route_type[8] = {'\0'};
    int conf_default_gateway[4] = {0};
    bool flag_eth = false;
    bool flag_route = false;

    char set_index_number[4] = {'\0'};
    char set_IP_addr[16] = {'\0'};
    char set_subnet_mask[16] = {'\0'};
    char set_default_gateway[16] = {'\0'};
    FILE *fp;

    /********Change language to English********/
    system("chcp 437");

    /********Get MAC address, index list********/
    MAX_MAC_index_num = get_interface_info(MAC_index_list);

    /********Open initif.conf********/
    if ((fp = fopen(file_name, "r")) == NULL)
    {
        printf("file open error!!\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(config_file_row, 256, fp) != NULL)
    {
        /********Get initif.conf********/
        temp_ptr = strtok(config_file_row, " ");
        if (strcmp(temp_ptr, "route") == 0)
        { //route
            temp_ptr = strtok(NULL, " ");
            strcpy(route_type, temp_ptr);
            //first octet
            temp_ptr = strtok(NULL, ".");
            conf_default_gateway[0] = strtol(temp_ptr, NULL, 10);
            //second octet
            temp_ptr = strtok(NULL, ".");
            conf_default_gateway[1] = strtol(temp_ptr, NULL, 10);
            //therd octet
            temp_ptr = strtok(NULL, ".");
            conf_default_gateway[2] = strtol(temp_ptr, NULL, 10);
            //forth octet
            temp_ptr = strtok(NULL, "/");
            conf_default_gateway[3] = strtol(temp_ptr, NULL, 10);
            if (DEBUG)
            {
                printf("routing type:%s\n", route_type);
                printf("default gateway's IP address:%d.%d.%d.%d\n", conf_default_gateway[0], conf_default_gateway[1], conf_default_gateway[2], conf_default_gateway[3]);
            }
            //TODO:chack input value
            //TODO:if route_type isn't "default"
            flag_route = true;
        }
        else if (strncmp(temp_ptr, "eth", 3) == 0)
        { //ethX
            /*network IP address*/
            //first octet
            temp_ptr = strtok(NULL, ".");
            net_IP_addr[0] = strtol(temp_ptr, NULL, 10);
            //second octet
            temp_ptr = strtok(NULL, ".");
            net_IP_addr[1] = strtol(temp_ptr, NULL, 10);
            //therd octet
            temp_ptr = strtok(NULL, ".");
            net_IP_addr[2] = strtol(temp_ptr, NULL, 10);
            //forth octet
            temp_ptr = strtok(NULL, "/");
            net_IP_addr[3] = strtol(temp_ptr, NULL, 10);
            /*prefix*/
            temp_ptr = strtok(NULL, " ");
            prefix = strtol(temp_ptr, NULL, 10);
            /*MAC address*/
            //1st octet
            temp_ptr = strtok(NULL, ":");
            strcpy(conf_MAC_addr[0], temp_ptr);
            //2nd octet
            temp_ptr = strtok(NULL, ":");
            strcpy(conf_MAC_addr[1], temp_ptr);
            //3rd octet
            temp_ptr = strtok(NULL, ":");
            strcpy(conf_MAC_addr[2], temp_ptr);
            //4th octet
            temp_ptr = strtok(NULL, ":");
            strcpy(conf_MAC_addr[3], temp_ptr);
            //5th octet
            temp_ptr = strtok(NULL, ":");
            strcpy(conf_MAC_addr[4], temp_ptr);
            //6th octet
            temp_ptr = strtok(NULL, " ");
            strcpy(conf_MAC_addr[5], temp_ptr);
            /*VLAN number*/
            temp_ptr = strtok(NULL, " ");
            VLAN = strtol(temp_ptr, NULL, 10);
            if (DEBUG)
            {
                printf("network ip address:%d.%d.%d.%d\n", net_IP_addr[0], net_IP_addr[1], net_IP_addr[2], net_IP_addr[3]);
                printf("prefix:%d\n", prefix);
                printf("conf MAC address:%s:%s:%s:%s:%s:%s\n", conf_MAC_addr[0], conf_MAC_addr[1], conf_MAC_addr[2], conf_MAC_addr[3], conf_MAC_addr[4], conf_MAC_addr[5]);
                printf("VLAN:%d\n", VLAN);
            }
            //TODO:chack input value
            flag_eth = true;
        }
        else
        {
            continue;
        }
        if (flag_eth == true)
        {
            /*setup ip address*/
            calc_subnet_mask(prefix, set_subnet_mask);
            MAC_index_num=calc_index_number(conf_MAC_addr, MAC_index_list, MAX_MAC_index_num);
            snprintf(set_index_number, 15, "%d", MAC_index_list[MAC_index_num].index);
            calc_ip_address(net_IP_addr, prefix, MAC_index_list[MAC_index_num].INT_MAC_addr, set_IP_addr);
            calc_default_gw(net_IP_addr, prefix, MAC_index_list[MAC_index_num].INT_MAC_addr, set_default_gateway);
            setup_interface(set_index_number, set_IP_addr, set_subnet_mask, set_default_gateway);
            flag_eth = false;
            flag_route = false;
        }
    }
    fclose(fp);

    return 0;
}

int get_interface_info(struct MAC_index MAC_index_list[10])
{
    FILE *command_result;
    char command_result_row[2048];
    char temp[4] = "";
    int flag = 0, i = 0, j = 0;
    int MAC_index_num = 0;

    //execute command
    if ((command_result = _popen("route print", "r")) == NULL)
    {
        printf("file open error!!\n");
        exit(EXIT_FAILURE);
    }

    //get MAC address and index number
    while (fgets(command_result_row, 2048, command_result) != NULL)
    {
        if (flag == 1 && command_result_row[0] == '=')
        {
            flag = 0;
        }
        if (flag == 1)
        { //mac address and index number row
            //Initial value input and Initialize
            MAC_index_list[MAC_index_num].index = 0;
            for (i = 0; i < 6; i++)
            {
                strcpy(MAC_index_list[MAC_index_num].CH_MAC_addr[i], "00");
                MAC_index_list[MAC_index_num].INT_MAC_addr[i] = 0;
            }
            memset(temp, '\0', strlen(temp));
            i = 0;
            j = 0;

            // get index number
            while (isdigit(command_result_row[i]) == 0)
            {
                i++;
                continue;
            }
            while (isdigit(command_result_row[i]) != 0)
            {
                temp[j] = command_result_row[i];
                i++;
                j++;
            }
            MAC_index_list[MAC_index_num].index = atoi(temp);

            //initialize
            memset(temp, '\0', strlen(temp));
            j = 0;

            // get mac address
            for (; i < strlen(command_result_row); i++)
            {
                if (isdigit(command_result_row[i]) == 0 && (command_result_row[i] < 'a' || 'f' < command_result_row[i]))
                {
                    continue;
                }
                temp[0] = command_result_row[i];
                temp[1] = command_result_row[i + 1];
                temp[2] = '\0';
                strcpy(MAC_index_list[MAC_index_num].CH_MAC_addr[j], temp);
                MAC_index_list[MAC_index_num].INT_MAC_addr[j] = strtol(temp, NULL, 16);
                i++;
                j++;
                if (j == 6)
                {
                    break;
                }
            }
            if (DEBUG)
            {
                printf("index:%d\n", MAC_index_list[MAC_index_num].index);
                printf("CH  MAC address:%s:%s:%s:%s:%s:%s\n", MAC_index_list[MAC_index_num].CH_MAC_addr[0], MAC_index_list[MAC_index_num].CH_MAC_addr[1], MAC_index_list[MAC_index_num].CH_MAC_addr[2], MAC_index_list[MAC_index_num].CH_MAC_addr[3], MAC_index_list[MAC_index_num].CH_MAC_addr[4], MAC_index_list[MAC_index_num].CH_MAC_addr[5]);
                printf("INT MAC address:%d:%d:%d:%d:%d:%d\n", MAC_index_list[MAC_index_num].INT_MAC_addr[0], MAC_index_list[MAC_index_num].INT_MAC_addr[1], MAC_index_list[MAC_index_num].INT_MAC_addr[2], MAC_index_list[MAC_index_num].INT_MAC_addr[3], MAC_index_list[MAC_index_num].INT_MAC_addr[4], MAC_index_list[MAC_index_num].INT_MAC_addr[5]);
            }
            MAC_index_num++;
        }
        if (strstr(command_result_row, "Interface List") != NULL)
        {
            flag = 1;
        }
    }
    (void)_pclose(command_result);

    return MAC_index_num;
}

int calc_subnet_mask(int prefix, char set_subnet_mask[16]){
    char temp_subnet_mask[8] = {'\0'};
    int i = 0, j = 0;
    for (i = 1; i <= 32; i++){
        if (i % 8 == 0 && i != 0){
            if (i == 8){
                snprintf(set_subnet_mask, 15, "%d", binary_to_decimal(temp_subnet_mask));
            }else{
                snprintf(set_subnet_mask, 15, "%s.%d", set_subnet_mask, binary_to_decimal(temp_subnet_mask));
            }
            memset(temp_subnet_mask, '\0', strlen(temp_subnet_mask));
            j = 0;
        }
        if (i < prefix){
            temp_subnet_mask[j] = '1';
        }else{
            temp_subnet_mask[j] = '0';
        }
        j++;
    }
    return 0;
}

int calc_index_number(char conf_MAC_addr[6][4], struct MAC_index MAC_index_list[10], int MAX_MAC_index_num){
    int i = 0;
    for (i = 0; i < MAX_MAC_index_num; i++){
        if (strcmp(MAC_index_list[i].CH_MAC_addr[4],conf_MAC_addr[4])==0){
            return i;
        }
    }
    return -1;
}

int calc_ip_address(int net_IP_addr[4], int prefix, int MAC_addr[6], char set_IP_addr[16]){
    char temp_subnet_mask[8] = {'\0'};
    int temp_int_subnet_mask = 0;
    int i = 0, j = 0;
    for (i = 1; i <= 32; i++){
        if (i <= prefix){
            temp_subnet_mask[j] = '1';
        }else{
            temp_subnet_mask[j] = '0';
        }
        j++;
        if (i % 8 == 0 && i != 0){
            temp_int_subnet_mask = binary_to_decimal(temp_subnet_mask);
            if (i == 8){
                snprintf(set_IP_addr, 16, "%d", (net_IP_addr[(i / 8) - 1] & temp_int_subnet_mask)+(MAC_addr[(i / 8) + 1] & ~temp_int_subnet_mask));
            }else{
                snprintf(set_IP_addr, 16, "%s.%d", set_IP_addr, (net_IP_addr[(i / 8) - 1] & temp_int_subnet_mask)+(MAC_addr[(i / 8) + 1] & ~temp_int_subnet_mask));
            }
            memset(temp_subnet_mask, '\0', strlen(temp_subnet_mask));
            j = 0;
        }
    }
    return 0;
}

int calc_default_gw(int net_IP_addr[4], int prefix, int MAC_addr[6], char set_default_gateway[16]){
    char temp_subnet_mask[8] = {'\0'};
    int temp_int_subnet_mask = 0;
    int i = 0, j = 0;
    for (i = 1; i <= 32; i++){
        if (i <= prefix){
            temp_subnet_mask[j] = '1';
        }else{
            temp_subnet_mask[j] = '0';
        }
        j++;
        if (i % 8 == 0 && i != 0){
            temp_int_subnet_mask = binary_to_decimal(temp_subnet_mask);
            if (i == 8){
                snprintf(set_default_gateway, 16, "%d", (net_IP_addr[(i / 8) - 1] & temp_int_subnet_mask)+(MAC_addr[(i / 8) + 1] & ~temp_int_subnet_mask));
            }else{
                snprintf(set_default_gateway, 16, "%s.%d", set_default_gateway, (net_IP_addr[(i / 8) - 1] & temp_int_subnet_mask)+(MAC_addr[(i / 8) + 1] & ~temp_int_subnet_mask));
            }
            if (i == 24){
                snprintf(set_default_gateway, 16, "%s.1", set_default_gateway);
                break;
            }
            memset(temp_subnet_mask, '\0', strlen(temp_subnet_mask));
            j = 0;
        }
    }
    return 0;
}

/*
used command
netsh interface ipv4 set address name=<index> source=static address=<IP address> mask=<subnet mask> gateway=<default gateway> gwmetric=1
//netsh interface ip set dns name=<index> static <gateway> primary
*/
void setup_interface(char set_index_number[4], char set_IP_addr[16], char set_subnet_mask[16], char set_default_gateway[16]){
    char cmd[256]={'\0'};
    if(DEBUG){
        printf("index:%s\n",set_index_number);
        printf("ip addr:%s\n",set_IP_addr);
        printf("sub net:%s\n",set_subnet_mask);
        printf("default GW:%s\n",set_default_gateway);
    }
    snprintf(cmd, 256, "netsh interface ipv4 set address name=%s source=static address=%s mask=%s gateway=%s gwmetric=1",set_index_number ,set_IP_addr, set_subnet_mask, set_default_gateway);
    printf("%s", cmd);
    system(cmd);
}

int binary_to_decimal(char binary_number[8])
{
    int return_number = 0;
    int i = 0;
    for (i = 0; i < 8; i++)
    {
        if (binary_number[i] != '0')
        {
            return_number += (int)pow(2, i);
        }
    }
    return return_number;
}