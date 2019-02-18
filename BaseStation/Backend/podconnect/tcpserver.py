from threading import Thread, Lock
from podconnect.models import DataPacket
import socket

from podconnect.models import DataPacket

def serve():
    TCP_IP = '127.0.0.1'
    TCP_PORT = 8001
    BUFFER_SIZE = 20

    mutex = Lock()
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
            try:
                data = conn.recv(BUFFER_SIZE)
                if not data or data == None:
                    break
                data = data.decode()
                dataSplit = data.split(',')
                d = DataPacket(velocity=dataSplit[0],acceleration=dataSplit[1],position=dataSplit[2])
                mutex.acquire(1)
                d.save()
                mutex.release()
                print("TCP received data:", data)
            except:
                print("Error in TCP Received message")

def start():
    t1 = Thread(target=serve)
    t1.start()
