#!/usr/bin/python3
# coding: utf-8

import yaml
import re

# YAML_PATH = "/etc/netplan/50-cloud-init.yaml"
YAML_PATH = "50-cloud-init.yaml.org"
interface_list = {}

with open(YAML_PATH) as stream:
    data = yaml.safe_load(stream)
    print(type(data))

for key, value in data["network"]["ethernets"].items():
    print(key)
    print(value)

with open("ip_a.txt", "r") as ipa:
    ipa_data = ipa.readlines()

is_skip = False
for data in ipa_data:
    pattern = '\d:.*'
    result = re.match(pattern, data)

    if result:
        if data.find("LOOPBACK") != -1:
            is_skip = True
        else:
            is_skip = False

    if is_skip:
        continue

    pattern = '(?P<name>\d):\s*(?P<int_name>.*:).*'
    result = re.match(pattern, data)
    if result:
        print(result.groups())

    pattern = 'link/ether\s*(?P<mac_addr>(\d | :)*)\s*.*'
    result = re.match(pattern, data)
    if result:
        print(result.groups())