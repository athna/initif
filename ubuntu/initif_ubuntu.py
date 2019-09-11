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

    def info(self):
        print("number:{}, name:{}, mac_addr:{}".format(self.number, self.name, self.mac_addr))

## get interface information
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

for interface in interface_list:
    interface.info()




## GET base file of netplan
with open("50-cloud-init.yaml.base") as stream:
    yaml_data = yaml.safe_load(stream)

print(yaml_data)