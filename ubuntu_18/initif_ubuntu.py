#!/usr/bin/python3
# coding: utf-8

import yaml
import re
import subprocess


class Interface():
    def __init__(self, int_number, int_name, mac_addr):
        self.number = int_number
        self.name = int_name
        self.mac_addr = mac_addr
        self.ipv4_addr = ""
        self.ipv6_addr = ""
        self.vlan = ""

    def set_addr(self, initif_datas):
        for initif_data in initif_datas:
            if self.mac_addr == initif_data[2]:
                if initif_data[1].find(".") != -1:
                    # ipv4
                    self.ipv4_addr = initif_data[1]
                elif initif_data[1].find(":") != -1:
                    # ipv6
                    self.ipv6_addr = initif_data[1]
                self.vlan = initif_data[3]
            elif initif_data[1] == "999.999.999.999/24":
                temp_mac_list = self.mac_addr.split(":")
                self.ipv4_addr = "{0}.{1}.{2}.{3}/24".format(
                    self.Base_16_to_10(temp_mac_list[2]),
                    self.Base_16_to_10(temp_mac_list[3]),
                    self.Base_16_to_10(temp_mac_list[4]),
                    self.Base_16_to_10(temp_mac_list[5]))
                return 1
        return 0

    def get_netplan_dict(self):
        '''
        enp3s0:
            addresses: [10.10.10.2/24, '2001:1::2/64']
            gateway4: 10.10.10.1
            nameservers:
                search: [mydomain, example.jp]
                addresses: [10.10.10.1, 1.1.1.1]
        '''
        if self.ipv4_addr == "" and self.ipv6_addr == "":
            return - 1

        temp_dict = {}
        temp_dict["addresses"] = []
        if self.ipv4_addr != "":
            temp_dict["addresses"].append(self.ipv4_addr)
        if self.ipv6_addr != "":
            temp_dict["addresses"].append(self.ipv6_addr)

        return temp_dict

    def Base_16_to_10(self, num_16: str) -> str:
        return str(int(num_16, 16))

    def info(self):
        print("***{}***".format(self.name))
        print("    - number:{}".format(self.number))
        print("    - mac_addr:{}".format(self.mac_addr))
        print("    - ipv4_addr:{}".format(self.ipv4_addr))
        print("    - ipv6_addr:{}".format(self.ipv6_addr))
        print("    - vlan:{}".format(self.vlan))


# get interface information
cmd = ("ip a")
ipa_data = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True).stdout.readlines()

is_skip = False
temp_number = ""
temp_int_name = ""
temp_mac_addr = ""
interface_list = []

for data in ipa_data:
    data = data.decode("utf-8")

    pattern = r'\d:.*'
    result = re.match(pattern, data)

    if result:
        if data.find("LOOPBACK") != -1:
            is_skip = True
        else:
            is_skip = False

    if is_skip:
        continue

    pattern = r'\s*(?P<number>\d+):\s*(?P<int_name>.*):.*'
    result = re.match(pattern, data)
    if result:
        temp_number = result.group("number")
        temp_int_name = result.group("int_name")

    pattern = r"(link/ether|link/ieee802.11)\s*(?P<mac_addr>(\d|:|[a-f])*)\s.*"
    result = re.search(pattern, data)
    if result:
        temp_mac_addr = result.group("mac_addr")

    if "" not in [temp_number, temp_int_name, temp_mac_addr]:
        interface_list.append(Interface(temp_number, temp_int_name, temp_mac_addr))
        temp_number = ""
        temp_int_name = ""
        temp_mac_addr = ""

# GET initif file
initif_datas = []
with open("initif2.conf") as initif_f:
    for data in initif_f.readlines():
        initif_datas.append(data.replace("\n", "").split(" "))

for interface in interface_list:
    result = interface.set_addr(initif_datas)
    if result == 1:
        break

# GET base file of netplan
with open("50-cloud-init.yaml.base") as stream:
    yaml_data = yaml.safe_load(stream)

# MAKE netplan file
yaml_data["network"]["ethernets"] = {}
for interface in interface_list:
    temp_data = interface.get_netplan_dict()
    if temp_data == -1:
        continue
    else:
        yaml_data["network"]["ethernets"][interface.name] = temp_data


with open("50-cloud-init.yaml", "w") as netplan_f:
    netplan_f.write(yaml.dump(yaml_data, default_flow_style=False))