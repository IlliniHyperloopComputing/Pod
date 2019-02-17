from threading import Thread, Lock
from podconnect.models import DataPacket
import socket

from podconnect.models import DataPacket

def serve():
    UDP_IP = '127.0.0.1'
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
    while(True):
        data, addr = s.recvfrom(BUFFER_SIZE)
        print(data)
    while (True):
        while (True):
            try:
                data, addr = s.recvfrom(BUFFER_SIZE)
                print(data)
                if not data or data == None:
                    break
                mutex.acquire(1)
                data = data.decode()
                dataSplit = data.split(',')
                d = DataPacket(velocity=dataSplit[1],acceleration=dataSplit[2],position=dataSplit[3])
                d.save()
                print("received data:", data, "from:", addr)
                mutex.release()
            except:
                print("Error in received message")
                mutex.release()
                break

def start():
    t1 = Thread(target=serve)
    t1.start()