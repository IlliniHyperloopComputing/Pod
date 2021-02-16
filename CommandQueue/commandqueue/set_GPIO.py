#!/usr/bin/env python

import command
from Utils import Utils

def set_GPIO(switchVal):

    switchVal = True

    is_GPIO_set = Utils.set_GPIO(Utils.WATCH_DOG_RESET_GPIO, True)
    if not is_GPIO_set:
        command.set_error_flag(command.Network_Command_ID.SET_OTHER_ERROR, command.OTHERErrors.GPIO_SWITCH_ERROR)

    #tests; delete when using
    assert Utils.WATCH_DOG_RESET_GPIO == 69
    assert command.Network_Command_ID.SET_OTHER_ERROR == 19
    assert command.OTHERErrors.GPIO_SWITCH_ERROR == 0x1
    network_command = command.Network_Command()
    command.get(network_command)
    assert network_command.id == 0

    is_GPIO_set = Utils.set_GPIO(Utils.HEARTBEAT_GPIO, switchVal)
    if not is_GPIO_set:
        command.set_error_flag(command.Network_Command_ID.SET_OTHER_ERROR, command.OTHERErrors.GPIO_SWITCH_ERROR)

    #tests; delete when using
    assert Utils.HEARTBEAT_GPIO == 68
    network_command = command.Network_Command()
    command.get(network_command)
    assert network_command.id == 0
    assert network_command.value == 0

    return not switchVal

set_GPIO(True)