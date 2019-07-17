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
                    data = tcphelper.bytes_to_signed_int32(data, 7)
                    if tcpsaver.saveADCData(data) == -1:
                        print("ADC data failure")
                elif id == 1: # CAN Data
                    data = conn.recv(45*4)
                    data = tcphelper.bytes_to_int(data, 45)
                    if tcpsaver.saveCANData(data) == -1:
                        print("CAN data failure")
                elif id == 2: # I2C Data
                    data = conn.recv(12*2)
                    data = tcphelper.bytes_to_int16(data, 12)
                    if tcpsaver.saveI2CData(data) == -1:
                        print("I2C data failure")
                elif id == 3: # PRU Data
                    data = conn.recv(4*4)
                    data = tcphelper.bytes_to_signed_int32(data, 4)
                    if tcpsaver.savePRUData(data) == -1:
                        print("PRU data failure")
                elif id == 4: # Motion Data
                    data = conn.recv(6*4 + 4)
                    data = tcphelper.bytes_to_signed_int32(data, 7)
                    if tcpsaver.saveMotionData(data) == -1:
                        print("Motion data failure")
                elif id == 5: # Error Data
                    data = conn.recv(6*4)
                    data = tcphelper.bytes_to_int(data, 6)
                    if tcpsaver.saveErrorData(data) == -1:
                        print("Error data failure")
                elif id == 6: # State Data
                    data = conn.recv(4)
                    data = tcphelper.bytes_to_int(data, 1)
                    if tcpsaver.saveStateData(data) == -1:
                        print("State data failure")
            except Exception as e:
                print(e)
                print("Error in TCP Received message")
                break
        print("Disconnected from Pod!!")
        # Add this to event logger

import binascii
def sendData():
    global conn, COMMAND_QUEUE
    # Sending data
    while True:
        if not COMMAND_QUEUE.empty() and conn != None:
            command = COMMAND_QUEUE.get()
            try:
                print("Sending " + str(command))
                for message in command:
                    convmessage = np.uint32(message)
                    conn.sendall(convmessage)
            except Exception as e:
                print(e)
                #COMMAND_QUEUE.put(command)
        time.sleep(0.2)

# Starts thread for tcp server and processor
def start():
    t1 = Thread(target=serve)
    t1.start()
    t2 = Thread(target=sendData)
    t2.start()

def addToCommandQueue(toSend):
    print(str(toSend) + " Added to Queue")
    COMMAND_QUEUE.put(toSend)