#!/usr/bin/env python

import command
from Utils import Utils

def set_GPIO(switchVal):

    switchVal = True

    is_GPIO_set = Utils.set_GPIO(69, True)
    if not is_GPIO_set:
        command.set_error_flag(19, 0x1)

    network_command = command.Network_Command()
    command.get(network_command)
    assert network_command.id == 0
    assert network_command.value == 0

    is_GPIO_set = Utils.set_GPIO(68, switchVal)
    if not is_GPIO_set:
        command.set_error_flag(19, 0x1)

    network_command = command.Network_Command()
    command.get(network_command)
    assert network_command.id == 0
    assert network_command.value == 0

    return not switchVal
