from threading import Thread
from . import models
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
TCP_IP = '127.0.0.1'
TCP_PORT = 8001
BUFFER_SIZE = 128

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

    # Receiving and sending data
    while (True):
        conn, addr = s.accept()
        print('Connection address:', addr)
        while (True):
            # Receiving data
            try:
                data = conn.recv(BUFFER_SIZE)
                if not data or data == None:
                    break
                h = bytearray(data)
                id = int(h[0])
                if id == 0:
                    # ToDo
                    pass
                elif id == 1: # CAN Data
                    if saveCANData(h[1:]) == -1:
                        print("CAN data failure")
                elif id == 2: # I2C Data
                    # ToDo
                    pass
                elif id == 3: # PRU Data
                    # ToDo
                    pass
                elif id == 4: # Motion Data
                    # ToDo
                    pass
                elif id == 5: # Error Data
                    if saveErrorData(h[1:]) == -1:
                        print("ADC data failure")
                elif id == 6: # State Data
                    if saveStateData(h[1:]) == -1:
                        print("State data failure")
            except:
                print("Error in TCP Received message")

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
                COMMAND_QUEUE.put(command)
        time.sleep(0.2)

def saveStateData(data):
    if len(data) < 1:
        return -1
    state_model = models.State(
        state = data[0]
    )
    state_model.save()

# Input: data array
# Function: saves data as a error data model
# Returns: -1 if the array is too small
# Returns: 1 on success
def saveErrorData(data):
    if len(data) < 6:
        return -1
    error_model = models.Errors(
        ADCError = data[0],
        CANError = data[1],
        I2CError = data[2],
        PRUError = data[3],
        NetworkError = data[4],
        OtherError = data[5],
    )
    error_model.save()

# There has to be a better way to do this
# Input: data array
# Function: saves data as a can data model
# Returns: -1 if the array is too small
# Returns: 1 on success
def saveCANData(data):
    if len(data) < 45:
        return -1
    can_model = models.CANData(
        # Motor Controller
        data = data[0],
        status_word = data[1],
        position_val = data[2],
        torque_val  = data[3],
        controller_temp = data[4],
        motor_temp = data[5],
        dc_link_voltage = data[6],
        logic_power_supply_voltage = data[6],
        current_demand = data[7],
        motor_current_val = data[8],
        electrical_angle = data[9],
        pdataase_a_current = data[10],
        pdataase_b_current = data[11],

        # BMS
        internal_relay_state = data[12],  # Used witdatain tdatae CANManager to set BMS relay states
        relay_state = data[13],           # Tdatais sdataould agree witdata tdatae above (given a small delay)
        rolling_counter = data[14],
        fail_safe_sate = data[15],
        peak_current = data[16],
        pack_voltage_inst = data[17],
        pack_voltage_open = data[18],
        pack_soc = data[19],
        pack_ampdataours = data[20],
        pack_resistance = data[21],
        pack_dod = data[22],
        pack_sodata = data[23],
        current_limit_status = data[24],
        max_pack_dcl = data[25],
        avg_pack_current = data[26],
        dataigdataest_temp = data[27],
        dataigdataest_temp_id = data[28],
        avg_temp = data[29],
        internal_temp = data[30],
        low_cell_voltge = data[31],
        low_cell_voltage_id = data[32],
        dataigdata_cell_voltage = data[33],
        dataigdata_cell_voltage_id = data[34],
        low_cell_internalR = data[35],
        low_cell_internalR_id = data[36],
        dataigdata_cell_internalR = data[37],
        dataigdata_cell_internalR_id = data[38],
        power_voltage_input = data[39],
        dtc_status_one = data[40],
        dtc_status_two = data[41],
        adaptive_total_cap = data[42],
        adaptive_ampdataours = data[43],
        adaptive_soc = data[44]
    )

    can_model.save()
    return 1

# Starts thread for tcp server
def start():
    t1 = Thread(target=serve)
    t1.start()
    t2 = Thread(target=sendData)
    t2.start()

def addToCommandQueue(toSend):
    print(str(toSend) + " Added to Queue")
    COMMAND_QUEUE.put(toSend)