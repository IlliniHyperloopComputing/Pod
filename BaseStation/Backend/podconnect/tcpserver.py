from threading import Thread, Lock
import socket

TCP_IP = '127.0.0.1'
TCP_PORT = 8001
BUFFER_SIZE = 20

def serve():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((TCP_IP, TCP_PORT))
    s.listen(1)
    while (True):
        conn, addr = s.accept()
        print('Connection address:', addr)
        while (True):
            data = conn.recv(BUFFER_SIZE)
            if not data:
                break
            print("received data:", data)

def start():
    t1 = Thread(target=serve)
    t1.start()