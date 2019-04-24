from threading import Thread, Lock, Event
from podconnect.models import DataPacket
import socket

def serve():
    UDP_IP = "127.0.0.1"
    UDP_PORT = 5005
    MSG_TO_SEND = "PING"
    MSG_TO_RECV = "ACK"

    e = Event() # Used instead of time.sleep()

    recv_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    recv_sock.bind((UDP_IP, 5004))
    recv_sock.settimeout(0.2) # Set timeout of recvfrom

    send_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP


    while (True):
        send_sock.sendto(MSG_TO_SEND.encode(), (UDP_IP, 5005))
        try:
            data, addr = recv_sock.recvfrom(1024) # buffer size is 1024 bytes
            if data.decode() == MSG_TO_RECV:
                print ("UDP: received correct message: ", data.decode())
            else:
                print ("UDP: received incorrect message: ", data.decode())
                # TODO: Determine what to do in this case! 
                # TODO: Does it mean the network is bad?
                # TODO: Should we throw an error??
        except:
            UDP_PORT = UDP_PORT + 1
    print("UDP Port = {port}".format(port=UDP_PORT))
    while (True):
        while (True):
            try:
                data, addr = s.recvfrom(BUFFER_SIZE)
                print(data)
                if not data or data == None:
                    break
                data = data.decode()
                print("UDP received data:", data, "From:", addr)
            except:
                print("Error in UDP received message")

        e.wait(timeout=0.3);
                
def start():
    t1 = Thread(target=serve)
    t1.start()
