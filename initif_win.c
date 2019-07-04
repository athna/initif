#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG false

// compile command
// gcc -O3 -mtune=native -march=native  -o initif .\initif_win.c

struct MAC_index {
    char CH_MAC_addr[32];
    int index;
};

int get_interface_info(struct MAC_index MAC_index_list[10]);
int compare_MAC_adder(struct MAC_index MAC_index_list[10], int MAX_MAC_index_num, char MAC_adder[32]);
void calc_subnet_mask(int prefix, char set_subnet_mask[16]);
void calc_default_gw(char ip_adder[16], int prefix, char set_default_gateway[16]);
void set_ipaddress(int set_index_number, char set_IP_addr[16], char set_subnet_mask[16], char set_default_gateway[16], int set_vlan);
void set_route(int set_index_number, char set_IP_addr[16], char set_subnet_mask[16], char set_default_gateway[16], int set_vlan);
int binary_to_decimal(char binary_number[8]);
void mac_to_ip_address(char MAC_adder[32], char set_ip_adder[16]);

int main(void) {
    char file_name[256] = "C:\\CyberRange\\initif\\initif.conf";
    FILE *fp;
    char *temp_ptr;
    char config_file_row[256] = {'\0'};
    int i = 0;

    struct MAC_index MAC_index_list[10];
    int MAX_MAC_index_num = 0;
    char conf_MAC_adder[32] = {'\0'};
    char conf_ip_adder[40] = {'\0'};
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

    if (MAX_MAC_index_num == -1) {
        return (-1);
    }

    /********Open initif.conf********/
    if ((fp = fopen(file_name, "r")) == NULL) {
        printf("file open error!!\n");
        exit(EXIT_FAILURE);
    }

    // read initif.conf
    while (fgets(config_file_row, 256, fp) != NULL) {
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

        // read row for initif.conf
        temp_ptr = strtok(config_file_row, " ");
        if (strcmp(temp_ptr, "#") == 0) {  // comment line
            continue;
        } else if (strncmp(temp_ptr, "eth", 3) == 0) {  // eth<X> line
            /*IP address*/
            temp_ptr = strtok(NULL, "/");
            strcpy(conf_ip_adder, temp_ptr);
            /*prefix*/
            temp_ptr = strtok(NULL, " ");
            conf_prefix = strtol(temp_ptr, NULL, 10);
            /*MAC address*/
            temp_ptr = strtok(NULL, " ");
            for (i = 0; i <= strlen(temp_ptr); i++) {
                /* アルファベットの大文字なら変換 */
                if (temp_ptr[i] >= 65 && temp_ptr[i] <= 90)
                    temp_ptr[i] = temp_ptr[i] + 32;
            }
            strcpy(conf_MAC_adder, temp_ptr);
            /*VLAN number*/
            temp_ptr = strtok(NULL, " ");
            conf_VLAN = strtol(temp_ptr, NULL, 10);
            if (DEBUG) {
                printf("ip address:%s\n", conf_ip_adder);
                printf("conf_prefix:%d\n", conf_prefix);
                printf("conf MAC address:%s\n", conf_MAC_adder);
                printf("conf_VLAN:%d\n", conf_VLAN);
            }
            // TODO:chack input value
        } else {  // another line
            continue;
        }

        // Generate value to assign to interface
        if (strncmp(conf_ip_adder, "999.999.999.999", 15) == 0) {  // make base image
            // prefix
            conf_prefix = 24;
            // MAC address and index number
            for (i = 0; i < MAX_MAC_index_num; i++) {
                if (strncmp(MAC_index_list[i].CH_MAC_addr, "52:54", 5) == 0) {
                    set_index_number = MAC_index_list[i].index;
                    strcpy(conf_MAC_adder, MAC_index_list[i].CH_MAC_addr);
                    break;
                }
            }
            if (set_index_number == 0) {
                return -1;
            }
            //// change MAC address to IP address
            mac_to_ip_address(conf_MAC_adder, temp_ip_adder);
            strcpy(set_ip_adder, temp_ip_adder);
            //// subnet mask
            calc_subnet_mask(conf_prefix, set_subnet_mask);
            //// default gateway
            calc_default_gw(temp_ip_adder, conf_prefix, set_default_gateway);

            // Assign some values to an interface
            set_ipaddress(set_index_number, set_ip_adder, set_subnet_mask, set_default_gateway, set_vlan);
            break;
        }

        // compare PC's MAC address with initif.comf's MAC address
        set_index_number = compare_MAC_adder(MAC_index_list, MAX_MAC_index_num, conf_MAC_adder);
        if (set_index_number != -1) {
            // calc value to assign to interface
            if (strchr(conf_ip_adder, (int)'.')) {  // IPv4
                //// ip address
                strcpy(set_ip_adder, conf_ip_adder);
                //// subnet mask
                calc_subnet_mask(conf_prefix, set_subnet_mask);
                //// default gateway
                calc_default_gw(conf_ip_adder, conf_prefix, set_default_gateway);

                // Assign some values to an interface
                set_ipaddress(set_index_number, set_ip_adder, set_subnet_mask, set_default_gateway, set_vlan);
            } else {  //IPv6
                puts("IPv6");
                //// ip address
                strcpy(set_ip_adder, conf_ip_adder);
                printf("IP:%s", set_ip_adder);
                // Assign some values to an interface
                set_ipaddress(set_index_number, set_ip_adder, set_subnet_mask, set_default_gateway, set_vlan);
            }
        }
    }
    return 0;
}

/**
 * @brief Get the interface info object
 * Store PC information in MAC_index_list
 *  - MAC address
 *  - index number
 * @param struct MAC_index
 * @return int
 * @retval -1   error
 * @retval 1<   Number of interfaces attached to this PC
 */
int get_interface_info(struct MAC_index MAC_index_list[10]) {
    FILE *command_result;
    char command_result_row[2048];
    char temp[32] = "";
    int flag = 0, i = 0, j = 0;
    int MAC_index_num = 0;

    /* execute command */
    if ((command_result = _popen("route print", "r")) == NULL) {
        printf("[error] couldn't get result for \"route print\" command\n");
        exit(-1);
    }

    /* get MAC address and index number */
    while (fgets(command_result_row, 2048, command_result) != NULL) {
        if (flag == 1 && command_result_row[0] == '=') {
            flag = 0;
        }

        if (flag == 1) {
            // mac address and index number row
            // Initial value input and Initialize
            MAC_index_list[MAC_index_num].index = 0;
            memset(MAC_index_list[MAC_index_num].CH_MAC_addr, '\0', strlen(MAC_index_list[MAC_index_num].CH_MAC_addr));
            memset(temp, '\0', strlen(temp));
            i = 0;
            j = 0;

            /********index number********/
            // get index number
            while (isdigit(command_result_row[i]) == 0) {
                i++;
                continue;
            }
            while (isdigit(command_result_row[i]) != 0) {
                temp[j] = command_result_row[i];
                i++;
                j++;
            }

            // if number on index is "1", this index is Loopback Interface.
            if (atoi(temp) == 1) {
                continue;
            }

            // set index number
            MAC_index_list[MAC_index_num].index = atoi(temp);

            /********MAC address********/
            // initialize
            memset(temp, '\0', strlen(temp));
            j = 0;

            // get MAC address
            for (; i < strlen(command_result_row); i++) {
                if (isdigit(command_result_row[i]) == 0 && (command_result_row[i] < 'a' || 'f' < command_result_row[i])) {
                    continue;
                } else if ('A' <= command_result_row[i] && command_result_row[i] <= 'F') {
                    temp[j] = command_result_row[i] + 32;
                    //temp[j + 1] = command_result_row[i + 1] + 32;
                } else {
                    temp[j] = command_result_row[i];
                    //temp[j + 1] = command_result_row[i + 1];
                }

                j++;
                if (j == 17) {
                    break;
                } else if ((j + 1) % 3 == 0) {
                    temp[j] = ':';
                    j++;
                }
            }
            // set MAC address
            strcpy(MAC_index_list[MAC_index_num].CH_MAC_addr, temp);

            // Exclude unsuitable MAC address
            if (j != 17) {
                continue;
            }

            if (DEBUG) {
                printf("index:%d,", MAC_index_list[MAC_index_num].index);
                printf("CH  MAC address:%s\n", MAC_index_list[MAC_index_num].CH_MAC_addr);
            }
            MAC_index_num++;
        }

        if (strstr(command_result_row, "Interface List") != NULL) {
            flag = 1;
        }
    }
    (void)_pclose(command_result);

    return MAC_index_num;
}

/**
 * @brief compare PC's MAC address with initif.comf's MAC address
 *
 * @param MAC_index_list
 * @param MAX_MAC_index_num
 * @param MAC_adder
 * @return int
 * @retval -1       Dont't match
 * @retval 1<       Index of interface that matching config file
 */
int compare_MAC_adder(struct MAC_index MAC_index_list[10], int MAX_MAC_index_num, char MAC_adder[32]) {
    int i;
    for (i = 0; i < MAX_MAC_index_num; i++) {
        if (strncmp(MAC_index_list[i].CH_MAC_addr, MAC_adder, 17) == 0) {
            return MAC_index_list[i].index;
        }
    }
    return -1;
}

/**
 * @brief Calculate subnet mask from prefix
 * ex) number of prefix = 24 --calc_subnet_mask()--> 255.255.255.0
 * @param prefix                Prefix used for calculation
 * @param set_subnet_mask       Assign the calculation result to this variable
 */
void calc_subnet_mask(int prefix, char set_subnet_mask[16]) {
    char temp_subnet_mask[8] = {'\0'};
    int i = 0, j = 0;
    for (i = 1; i <= 32; i++) {
        if (i % 8 == 0 && i != 0) {
            if (i == 8) {
                snprintf(set_subnet_mask, 15, "%d", binary_to_decimal(temp_subnet_mask));
            } else {
                snprintf(set_subnet_mask, 15, "%s.%d", set_subnet_mask, binary_to_decimal(temp_subnet_mask));
            }
            memset(temp_subnet_mask, '\0', strlen(temp_subnet_mask));
            j = 0;
        }
        if (i < prefix) {
            temp_subnet_mask[j] = '1';
        } else {
            temp_subnet_mask[j] = '0';
        }
        j++;
    }
}

/**
 * @brief Calculate default gateway from IP address and prefix length
 * The IP address of the gateway has a final octet of 1
 * @param ip_adder              IP address used for calculation
 * @param prefix                prefix used for calculation
 * @param set_default_gateway   Assign the calculation result to this variable
 */
void calc_default_gw(char ip_adder[16], int prefix, char set_default_gateway[16]) {
    char *temp_ptr;

    // first octet
    temp_ptr = strtok(ip_adder, ".");
    strcat(set_default_gateway, temp_ptr);
    strcat(set_default_gateway, ".");
    // second octet
    temp_ptr = strtok(NULL, ".");
    strcat(set_default_gateway, temp_ptr);
    strcat(set_default_gateway, ".");
    // therd octet
    temp_ptr = strtok(NULL, ".");
    strcat(set_default_gateway, temp_ptr);
    strcat(set_default_gateway, ".");
    // forth octet
    temp_ptr = strtok(NULL, ".");
    strcat(set_default_gateway, "1");
}

/**
 * TODO: Implement
 * @brief Assign route to an interface
 *
 * @param set_index_number      Index value of interface to set
 * @param set_IP_addr           IP address to assign
 * @param set_subnet_mask       Subnet mask to assign
 * @param set_default_gateway   default gateway to assign
 * @param set_vlan              vlan to assign (Don't support now)
 *
 * @detail
 * Used Windows command
 * netsh interface ipv4 set address name=<index> source=static address=<IP address>
 * mask=<subnet mask> gateway=<default gateway> gwmetric=1
 */
void set_route(int set_index_number, char set_IP_addr[16], char set_subnet_mask[16], char set_default_gateway[16], int set_vlan) {
    char cmd[256] = {'\0'};
    if (DEBUG) {
        printf("index:%d\n", set_index_number);
        printf("ip addr:%s\n", set_IP_addr);
        printf("sub net:%s\n", set_subnet_mask);
        printf("default GW:%s\n", set_default_gateway);
    }

    if (strchr(set_IP_addr, (int)'.')) {  // IPv4
        snprintf(
            cmd, 256,
            "netsh interface ipv4 set address name=%d source=static address=%s "
            "mask=%s gateway=%s gwmetric=1",
            set_index_number, set_IP_addr, set_subnet_mask, set_default_gateway);
    } else {  // IPv6
        snprintf(
            cmd, 256,
            "netsh interface ipv6 add route prefix=%s interface=%d publish=no",
            set_default_gateway, set_index_number);
        // default gateway?
        // https://www.computernetworkingnotes.com/networking-tutorials/how-to-configure-ipv6-address-in-windows.html
        // コマンドの結合は&っぽい
    }
    if (DEBUG) {
        printf("%s", cmd);
    }
    system(cmd);
}

/**
 * @brief Assign ip address to an interface
 *
 * @param set_index_number      Index value of interface to set
 * @param set_IP_addr           IP address to assign
 * @param set_subnet_mask       Subnet mask to assign
 * @param set_default_gateway   default gateway to assign
 * @param set_vlan              vlan to assign (Don't support now)
 *
 * @detail
 * Used Windows command
 * netsh interface ipv4 set address name=<index> source=static address=<IP address>
 * mask=<subnet mask> gateway=<default gateway> gwmetric=1
 */
void set_ipaddress(int set_index_number, char set_IP_addr[16], char set_subnet_mask[16], char set_default_gateway[16], int set_vlan) {
    char cmd[256] = {'\0'};
    if (DEBUG) {
        printf("index:%d\n", set_index_number);
        printf("ip addr:%s\n", set_IP_addr);
        printf("sub net:%s\n", set_subnet_mask);
        printf("default GW:%s\n", set_default_gateway);
    }

    if (strchr(set_IP_addr, (int)'.')) {  // IPv4
        snprintf(
            cmd, 256,
            "netsh interface ipv4 set address name=%d source=static address=%s "
            "mask=%s gateway=%s gwmetric=1",
            set_index_number, set_IP_addr, set_subnet_mask, set_default_gateway);
    } else {  // IPv6
        snprintf(
            cmd, 256,
            "netsh interface ipv6 set address interface=%d address=%s "
            "type=unicast validlifetime=infinite preferredlifetime=infinite store=active",
            set_index_number, set_IP_addr);
    }
    if (DEBUG) {
        printf("%s", cmd);
    }
    system(cmd);
}

/**
 * @brief Convert a binary number to a decimal number
 * ex) 11110000 --binary_to_decimal()--> 240
 * @attention Only 8bit can be used
 * @param binary_number
 * @return int
 */
int binary_to_decimal(char binary_number[8]) {
    int return_number = 0;
    int i = 0;
    for (i = 0; i < 8; i++) {
        if (binary_number[i] != '0') {
            return_number += (int)pow(2, i);
        }
    }
    return return_number;
}

/**
 * @brief Calculate IP address from MAC address
 *
 * @param MAC_adder         MAC address used for calculation
 * @param set_ip_adder      Assign the calculation result to this variable
 */
void mac_to_ip_address(char MAC_adder[32], char set_ip_adder[16]) {
    char *temp_ptr;

    temp_ptr = strtok(MAC_adder, ":");  // first octet
    temp_ptr = strtok(NULL, ":");       // second octet
    temp_ptr = strtok(NULL, ":");       // thard octet
    sprintf(set_ip_adder, "%ld", strtol(temp_ptr, NULL, 16));
    temp_ptr = strtok(NULL, ":");  // foth octet
    sprintf(set_ip_adder, "%s.%ld", set_ip_adder, strtol(temp_ptr, NULL, 16));
    temp_ptr = strtok(NULL, ":");  // fifth octet
    sprintf(set_ip_adder, "%s.%ld", set_ip_adder, strtol(temp_ptr, NULL, 16));
    temp_ptr = strtok(NULL, ":");  // sixth octet
    sprintf(set_ip_adder, "%s.%ld", set_ip_adder, strtol(temp_ptr, NULL, 16));
    printf("set_ip_adder:%s", set_ip_adder);
}