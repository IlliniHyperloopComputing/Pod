from threading import Thread
from . import tcpsaver, tcphelper
import socket
import queue
import time
import numpy as np

# TCP IDs:
# uint8_t adc_id = 0;
# uint8_t can_id = 1;
# uint8_t i2c_id = 2;
# uint8_t pru_id = 3;
# uint8_t motion_id = 4;
# uint8_t error_id = 5;
# uint8_t state_id = 6;

# TCP global variables
TCP_IP = ''
TCP_PORT = 8001
BUFFER_SIZE = 300

conn = None
addr = None

# Initialize command queue
COMMAND_QUEUE = queue.Queue()

def serve():
    global TCP_IP, TCP_PORT, BUFFER_SIZE, conn, addr

    #Socket setup
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    bound = False
    while (not bound):
        try:
            s.bind((TCP_IP, TCP_PORT))
            bound = True
        except:
            TCP_PORT = TCP_PORT + 1
    print("TCP Port = {port}".format(port=TCP_PORT))
    s.listen(1)

    while (True):
        conn, addr = s.accept()
        print('Connection address:', addr)
        while (True):
            # Receiving data
            try:
                data = conn.recv(1)
                if not data or data == None:
                    break
                h = bytearray(data)
                id = int(h[0])
                if id == 7: # ADC Data
                    data = conn.recv(7*4)
                elif id == 1: # CAN Data
                    data = conn.recv(45*4)
                elif id == 2: # I2C Data
                    data = conn.recv(12*2)
                elif id == 3: # PRU Data
                    data = conn.recv(4*4)
                elif id == 4: # Motion Data
                    data = conn.recv(6*4 + 8*8 + 4)
                elif id == 5: # Error Data
                    data = conn.recv(6*4)
                elif id == 6: # State Data
                    data = conn.recv(4)
                elif id == 9:
                    input("Press enter to get next:")
                    data = conn.recv(30*(1 + 3*2 + 1) + 48)
                    readCell(data[:30*(1 + 3*2 + 1)])
                    data_int8 = tcphelper.bytes_to_uint8(data[30*(1+3*2+1):-40], 8)
                    print("num_therms_enabled: " + data_int8[0])
                    print("highest_therm_value: " + data_int8[2])
                    print("highest_therm_id: " + data_int8[3])
                    print("lowest_therm_value: " + data_int8[1])
                    print("lowest_therm_id: " + data_int8[4])
                    print("PADDING: " + data_int8[5])
                    print("PADDING2: " + data_int8[6])
                    print("PADDING3: " + data_int8[7])

                    data_int8 = tcphelper.bytes_to_int8(data[-40:], 40)
                    for i in range(40):
                        print("Therm " + i + ": " + data_int8[i])
            except Exception as e:
                print(e)
                print("Error in TCP Received message")
                break
        print("Disconnected from Pod!!")
        # Add this to event logger


def readCell(data):
    if len(data) != 1 + 3*2 + 1:
        print("Failure")
        return
    cell_id = int.from_bytes(data[0:1], byteorder='little', signed=True)
    print("Cell: " + cell_id,end=" ")
    voltage_stuff = tcphelper.bytes_to_uint16(data[1:6])
    print("Instant_Voltage: " + cell_id,end=" ")
    print("Instant_Resistance: " + cell_id,end=" ")
    print("Open_Voltage: " + cell_id,end=" ")
    checksum = int.from_bytes(data[-1:], byteorder='little', signed=True)
    print("Checksum: " + checksum)
    return
