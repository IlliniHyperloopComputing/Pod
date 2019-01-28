from threading import Thread, Lock
from podconnect.models import DataPacket
import socket

from podconnect.models import DataPacket

TCP_IP = '127.0.0.1'
TCP_PORT = 8001
BUFFER_SIZE = 20

def serve():
    mutex = Lock()
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((TCP_IP, TCP_PORT))
    s.listen(1)
    while (True):
        conn, addr = s.accept()
        print('Connection address:', addr)
        while (True):
            data = conn.recv(BUFFER_SIZE)
            if not data or data == None:
                break
            mutex.acquire(1)
            data = data.decode()
            dataSplit = data.split(',')
            d = DataPacket(velocity=dataSplit[1],acceleration=dataSplit[2],position=dataSplit[3])
            d.save()
            print("received data:", data)
            mutex.release()

def start():
    t1 = Thread(target=serve)
    t1.start()