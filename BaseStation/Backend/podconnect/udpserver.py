from threading import Thread, Lock, Event
import numpy as np
from . import models
import time, socket, queue

# Initialize command queue
COMMAND_QUEUE = queue.Queue()

send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
UDP_RECV_IP = ''
UDP_SEND_IP = '192.168.1.60' #127.0.0.1'
UDP_SEND_PORT = 5005
UDP_RECV_PORT = 5004
MSG_TO_SEND = "PING"
MSG_TO_RECV = "ACK"

def serve():
    global send_sock, UDP_RECV_IP, UDP_SEND_IP, UDP_SEND_PORT, UDP_RECV_PORT, MSG_TO_RECV, MSG_TO_SEND
    

    e = Event() # Used instead of time.sleep()

    recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    recv_sock.bind((UDP_RECV_IP, UDP_RECV_PORT))
    recv_sock.settimeout(0.2) # Set timeout of recvfrom

    

    print("UDP Port = {port}".format(port=UDP_SEND_PORT))
    while (True):
        send_sock.sendto(MSG_TO_SEND.encode(), (UDP_SEND_IP, UDP_SEND_PORT))
        try:
            data, addr = recv_sock.recvfrom(1024) # buffer size is 1024 bytes
            if data.decode() == MSG_TO_RECV:
                # wait a small ammount of time, as to not cause a flood of messages back and forth
                e.wait( timeout=0.020); 
                print ("UDP: (comment me out) received correct message: ", data.decode())
            else:
                print ("UDP: received incorrect message: ", data.decode())
            #    # TODO: Determine what to do in this case! 
            #    # TODO: Does it mean the network is bad?
            #    # TODO: Should we throw an error??
        except Exception as f:
            #print(f)
            pass

def sendData():
    global send_sock, COMMAND_QUEUE, UDP_SEND_IP, UDP_SEND_PORT
    # Sending data
    while True:
        if not COMMAND_QUEUE.empty():
            command = COMMAND_QUEUE.get()
            try:
                print("Sending " + str(command))
                for message in command:
                    convmessage = np.uint32(message)
                    send_sock.sendto(convmessage, (UDP_SEND_IP, UDP_SEND_PORT))
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
