from threading import Thread, Lock
from podconnect.models import DataPacket
import socket

from podconnect.models import DataPacket

def serve():
    UDP_IP = "127.0.0.1"
    UDP_PORT = 5005
    MESSAGE = "PING"

    sock2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    sock2.bind(UDP_IP, 5004)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP

    sock2.settimeout(0.2) # Set timeout of recvfrom

    while (True):
        sock.sendto(MESSAGE, (UDP_IP, 5005))
        try:
	    data, addr = sock2.recvfrom(1024) # buffer size is 1024 bytes
	    print ("SENDERE: received message: ", data)
	    time.sleep(.9)
	except:
	    print ("Never received message")

                
def start():
    t1 = Thread(target=serve)
    t1.start()
