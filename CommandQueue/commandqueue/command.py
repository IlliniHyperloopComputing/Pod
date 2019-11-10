#!/usr/bin/env python

import command
print("import success")
command.put(100, 100)
print("push success")
network_command = command.Network_Command()
if command.get(network_command):
    print("pop success")
    print(network_command.id)
    print(network_command.value)
