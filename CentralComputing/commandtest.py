from ctypes import *
commandlib = CDLL("./commandlib.so")
commandlib.connect()

print(vars(commandlib))
print(commandlib.Command)
print(commandlib.get_network_command_ID_string())
