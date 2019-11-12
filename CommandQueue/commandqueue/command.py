#!/usr/bin/env python

import command
print("import success")
command.put(100, 100)
print("push success")
network_command = command.Network_Command()
assert command.get(network_command)
print("pop success")
assert network_command.id == 100
assert network_command.value == 100
print(network_command.id)
print(network_command.value)