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
bms_cell_broadcast_data = command.BMSCellBroadcastData()
print("BMSCellBroadcastData Class Available")

bms_cell_broadcast_data.cell_id = 37
assert bms_cell_broadcast_data.cell_id == 37
print("BMSCellBroadcastData Class Variables Accessible")

# test BMSCells
bms_cells = command.BMSCells()
print("BMSCells Class Available")

bms_cells.num_therms_enabled = 37
assert bms_cells.num_therms_enabled == 37
print("BMSCells Class Variables Accessible")

# test I2CData
i2c_data = command.I2CData()
print("I2CData Class Available")

i2c_data.pressure_sensor = 37
assert i2c_data.pressure_sensor == 37
print("I2CData Class Variables Accessible")

# test PRUData
pru_data = command.PRUData()
print("PRUData Class Available")

pru_data.watchdog_hz = 37
assert pru_data.watchdog_hz == 37
print("PRUData Class Variables Accessible")

# test MotionData
motion_data = command.MotionData()
print("MotionData Class Available")

motion_data.p_timeout = 37
assert motion_data.p_timeout == 37
print("MotionData Class Variables Accessible")