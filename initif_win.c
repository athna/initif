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
    char CH_MAC_addr[32];
    int index;
};

int get_interface_info(struct MAC_index MAC_index_list[10]);
int compare_MAC_adder(struct MAC_index MAC_index_list[10], int MAX_MAC_index_num, char MAC_adder[32]);
int calc_subnet_mask(int prefix, char set_subnet_mask[16]);
int calc_default_gw(char ip_adder[16], int prefix, char set_default_gateway[16]);
void setup_interface(int set_index_number, char set_IP_addr[16], char set_subnet_mask[16], char set_default_gateway[16], int set_vlan);
int binary_to_decimal(char binary_number[8]);
void mac_to_ip_address(char MAC_adder[32], char set_ip_adder[16]);

int main(void)
{
    char file_name[256] = "C:\\CyberRange\\initif\\initif.conf";
    FILE *fp;
    char *temp_ptr;
    char config_file_row[256] = {'\0'};
    int i=0;

    struct MAC_index MAC_index_list[10];
    int MAX_MAC_index_num = 0;
    char conf_MAC_adder[32] = {'\0'};
    char conf_ip_adder[16] = {'\0'};
    char temp_ip_adder[16] = {'\0'};
    int conf_prefix = 0;
    char conf_gw_ip_adder[16] = {'\0'};
    int conf_VLAN = 0;

    int set_index_number = {0};
    char set_ip_adder[16] = {'\0'};
    char set_subnet_mask[16] = {'\0'};
    char set_default_gateway[16] = {'\0'};
    int set_vlan = 0;

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
        /******** initialise ******/
        memset(conf_MAC_adder, '\0', strlen(conf_MAC_adder));
        memset(conf_ip_adder, '\0', strlen(conf_ip_adder));
        memset(temp_ip_adder, '\0', strlen(temp_ip_adder));
        memset(conf_gw_ip_adder, '\0', strlen(conf_gw_ip_adder));
        memset(set_ip_adder, '\0', strlen(set_ip_adder));
        memset(set_subnet_mask, '\0', strlen(set_subnet_mask));
        memset(set_default_gateway, '\0', strlen(set_default_gateway));
        conf_prefix = 0;
        conf_VLAN = 0;
        set_vlan = 0;

        /********Get initif.conf********/
        temp_ptr = strtok(config_file_row, " ");
        // comment line
        if (strcmp(temp_ptr, "#") == 0)
        {
            continue;
        }
        // eth<X> line
        else if (strncmp(temp_ptr, "eth", 3) == 0)
        {
            /*IP address*/
            temp_ptr = strtok(NULL, "/");
            strcpy(conf_ip_adder, temp_ptr);
            /*prefix*/
            temp_ptr = strtok(NULL, " ");
            conf_prefix = strtol(temp_ptr, NULL, 10);
            /*MAC address*/
            temp_ptr = strtok(NULL, " ");
            strcpy(conf_MAC_adder, temp_ptr);
            /*VLAN number*/
            temp_ptr = strtok(NULL, " ");
            conf_VLAN = strtol(temp_ptr, NULL, 10);
            if (DEBUG)
            {
                printf("ip address:%s\n", conf_ip_adder);
                printf("conf_prefix:%d\n", conf_prefix);
                printf("conf MAC address:%s\n", conf_MAC_adder);
                printf("conf_VLAN:%d\n", conf_VLAN);
            }
            //TODO:chack input value
        }
        // another line
        else
        {
            continue;
        }

        /* compare */
        // is maked base image faith
        if (strncmp(conf_ip_adder, "999.999.999.999",15) == 0)
        {
            // prefix
            conf_prefix = 24;
            // MAC address and index number
            for (i = 0; i < MAX_MAC_index_num;i++){
                if (strncmp(MAC_index_list[i].CH_MAC_addr,"52:54",5)==0){
                    set_index_number = MAC_index_list[i].index;
                    strcpy(conf_MAC_adder, MAC_index_list[i].CH_MAC_addr);
                }
            }
            if (set_index_number==0){
                return -1;
            }
            // change MAC address to IP address
            mac_to_ip_address(conf_MAC_adder, temp_ip_adder);
            strcpy(set_ip_adder, temp_ip_adder);
            // subnet mask
            calc_subnet_mask(conf_prefix, set_subnet_mask);
            // default gateway
            calc_default_gw(temp_ip_adder, conf_prefix, set_default_gateway);

            /* setup interface */
            setup_interface(set_index_number, set_ip_adder, set_subnet_mask, set_default_gateway, set_vlan);
            break;
        }
        // compare MAC address
        set_index_number = compare_MAC_adder(MAC_index_list, MAX_MAC_index_num, conf_MAC_adder);
        if (set_index_number != -1)
        {
            /* calc same value */
            // ip address
            strcpy(set_ip_adder, conf_ip_adder);
            // subnet mask
            calc_subnet_mask(conf_prefix, set_subnet_mask);
            // default gateway
            calc_default_gw(conf_ip_adder, conf_prefix, set_default_gateway);

            /* setup interface */
            setup_interface(set_index_number, set_ip_adder, set_subnet_mask, set_default_gateway, set_vlan);
        }
    }
    return 0;
}

int get_interface_info(struct MAC_index MAC_index_list[10])
{
    FILE *command_result;
    char command_result_row[2048];
    char temp[32] = "";
    int flag = 0, i = 0, j = 0;
    int MAC_index_num = 0;

    /* execute command */
    if ((command_result = _popen("route print", "r")) == NULL)
    {
        printf("file open error!!\n");
        exit(EXIT_FAILURE);
    }

    /* get MAC address and index number */
    while (fgets(command_result_row, 2048, command_result) != NULL)
    {
        if (flag == 1 && command_result_row[0] == '=')
        {
            flag = 0;
        }
        if (flag == 1)
        {
            //mac address and index number row
            //Initial value input and Initialize
            MAC_index_list[MAC_index_num].index = 0;
            memset(MAC_index_list[MAC_index_num].CH_MAC_addr, '\0', strlen(MAC_index_list[MAC_index_num].CH_MAC_addr));
            memset(temp, '\0', strlen(temp));
            i = 0;
            j = 0;

            /********index number********/
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
            // set index number
            MAC_index_list[MAC_index_num].index = atoi(temp);

            /********MAC address********/
            //initialize
            memset(temp, '\0', strlen(temp));
            j = 0;

            // get MAC address
            for (; i < strlen(command_result_row); i++)
            {
                if (isdigit(command_result_row[i]) == 0 && (command_result_row[i] < 'a' || 'f' < command_result_row[i]))
                {
                    continue;
                }
                else if (command_result_row[i] >= 65 && command_result_row[i] <= 90)
                {
                    temp[j] = command_result_row[i] + 32;
                    temp[j + 1] = command_result_row[i + 1] + 32;
                }
                else
                {
                    temp[j] = command_result_row[i];
                    temp[j + 1] = command_result_row[i + 1];
                }
                if (j == 15)
                {
                    break;
                }
                temp[j + 2] = ':';
                i++;
                j += 3;
            }
            // set MAC address
            strcpy(MAC_index_list[MAC_index_num].CH_MAC_addr, temp);

            // Exclude unsuitable MAC address
            if (j != 15)
            {
                continue;
            }

            if (DEBUG)
            {
                printf("index:%d,", MAC_index_list[MAC_index_num].index);
                printf("CH  MAC address:%s\n", MAC_index_list[MAC_index_num].CH_MAC_addr);
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

int compare_MAC_adder(struct MAC_index MAC_index_list[10], int MAX_MAC_index_num, char MAC_adder[32])
{
    int i;
    for (i = 0; i < MAX_MAC_index_num; i++)
    {
        if (strncmp(MAC_index_list[i].CH_MAC_addr, MAC_adder, 17) == 0)
        {
            return MAC_index_list[i].index;
        }
    }
    return -1;
}

int calc_subnet_mask(int prefix, char set_subnet_mask[16])
{
    char temp_subnet_mask[8] = {'\0'};
    int i = 0, j = 0;
    for (i = 1; i <= 32; i++)
    {
        if (i % 8 == 0 && i != 0)
        {
            if (i == 8)
            {
                snprintf(set_subnet_mask, 15, "%d", binary_to_decimal(temp_subnet_mask));
            }
            else
            {
                snprintf(set_subnet_mask, 15, "%s.%d", set_subnet_mask, binary_to_decimal(temp_subnet_mask));
            }
            memset(temp_subnet_mask, '\0', strlen(temp_subnet_mask));
            j = 0;
        }
        if (i < prefix)
        {
            temp_subnet_mask[j] = '1';
        }
        else
        {
            temp_subnet_mask[j] = '0';
        }
        j++;
    }
    return 0;
}

/*The IP address of the gateway has a final octet of 1*/
int calc_default_gw(char ip_adder[16], int prefix, char set_default_gateway[16])
{
    char *temp_ptr;

    //first octet
    temp_ptr = strtok(ip_adder, ".");
    strcat(set_default_gateway, temp_ptr);
    strcat(set_default_gateway, ".");
    //second octet
    temp_ptr = strtok(NULL, ".");
    strcat(set_default_gateway, temp_ptr);
    strcat(set_default_gateway, ".");
    //therd octet
    temp_ptr = strtok(NULL, ".");
    strcat(set_default_gateway, temp_ptr);
    strcat(set_default_gateway, ".");
    //forth octet
    temp_ptr = strtok(NULL, ".");
    strcat(set_default_gateway, "1");

    return 0;
}

/*
used command
netsh interface ipv4 set address name=<index> source=static address=<IP address> mask=<subnet mask> gateway=<default gateway> gwmetric=1
*/
void setup_interface(int set_index_number, char set_IP_addr[16], char set_subnet_mask[16], char set_default_gateway[16], int set_vlan)
{
    char cmd[256] = {'\0'};
    if (DEBUG)
    {
        printf("index:%d\n", set_index_number);
        printf("ip addr:%s\n", set_IP_addr);
        printf("sub net:%s\n", set_subnet_mask);
        printf("default GW:%s\n", set_default_gateway);
    }
    snprintf(cmd, 256, "netsh interface ipv4 set address name=%d source=static address=%s mask=%s gateway=%s gwmetric=1", set_index_number, set_IP_addr, set_subnet_mask, set_default_gateway);
    //printf("%s", cmd);
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

void mac_to_ip_address(char MAC_adder[32], char set_ip_adder[16]){
    int i = 0;
    long temp = 0;
    char *temp_ptr;

    temp_ptr = strtok(MAC_adder, ":");//first octet
    temp_ptr = strtok(NULL, ":");//second octet
    temp_ptr = strtok(NULL, ":");//thard octet
    sprintf(set_ip_adder, "%ld", strtol(temp_ptr, NULL, 16));
    temp_ptr = strtok(NULL, ":");//foth octet
    sprintf(set_ip_adder, "%s.%ld", set_ip_adder, strtol(temp_ptr, NULL, 16));
    temp_ptr = strtok(NULL, ":");//fifth octet
    sprintf(set_ip_adder, "%s.%ld", set_ip_adder, strtol(temp_ptr, NULL, 16));
    temp_ptr = strtok(NULL, ":");//sixth octet
    sprintf(set_ip_adder, "%s.%ld", set_ip_adder, strtol(temp_ptr, NULL, 16));
    printf("set_ip_adder:%s", set_ip_adder);
}