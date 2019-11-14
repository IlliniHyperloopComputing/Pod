#!/usr/bin/env python

import command

print("import success")

# test command queue functions
network_command = command.Network_Command()
for x in range(100000):
    command.put(x, x)

for x in range(100000):
    assert command.get(network_command)
    assert network_command.id == x
    assert network_command.value == x

# test ADCData
adc_data = command.ADCData()
print("ADCData Class Available")

# test CANData
can_data = command.CANData()
print("CANData Class Available")

can_data.status_word = 37
assert can_data.status_word == 37
print("CANData Class Variables Accessible")

# test BMSCellBroadcastData
BMS_cell_broadcast_data = command.BMSCellBroadcastData()
print("BMSCellBroadcastData Class Available")

BMS_cell_broadcast_data.cell_id = 37
assert BMS_cell_broadcast_data.cell_id == 37
print("BMSCellBroadcastData Class Variables Accessible")

# test BMSCells
BMS_cells = command.BMSCells()
print("BMSCells Class Available")

BMS_cells.num_therms_enabled = 37
assert BMS_cells.num_therms_enabled == 37
print("BMSCells Class Variables Accessible")

# test I2CData
I2C_data = command.I2CData()
print("I2CData Class Available")

I2C_data.pressure_sensor = 37
assert I2C_data.pressure_sensor == 37
print("I2CData Class Variables Accessible")
