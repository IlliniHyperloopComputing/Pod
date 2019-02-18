from threading import Thread, Lock
from podconnect.models import DataPacket
import socket

from podconnect.models import DataPacket

def serve():
    UDP_IP = ''
    UDP_PORT = 8002
    BUFFER_SIZE = 20

    mutex = Lock()
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    bound = False
    while (not bound):
        try:
            s.bind((UDP_IP, UDP_PORT))
            bound = True
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
                dataSplit = data.split(',')
                d = DataPacket(velocity=dataSplit[1],acceleration=dataSplit[2],position=dataSplit[3])
                mutex.acquire(1)
                d.save()
                mutex.release()
                print("UDP received data:", data, "From:", addr)
            except:
                print("Error in UDP received message")

def start():
    t1 = Thread(target=serve)
    t1.start()