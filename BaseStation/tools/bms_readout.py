import socket

from . imprt tcphelper

TCP_IP = ''
TCP_PORT = 8001

def serve():
    global TCP_IP, TCP_PORT

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

                if id == 9:
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

                    data_int8 = tcphelper.bytes_to_int8(datta[-40:], 40)
                    for i in range(40):
                        print("Therm " + i + ": " + data_int8[i])


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
