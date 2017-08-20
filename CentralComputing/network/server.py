import time
import socket
import sys
from _thread import *

def threaded_client(conn, addr):
    conn.send((1024).to_bytes(2,byteorder='big'))
    #conn.send(str.encode('5elcome, asshole from ' + addr[0] + ":" + str(addr[1])  + '\n'))

    while True:
        try:
            data = conn.recv(2048).decode()
            print("from <" + addr[0] + ':' + str(addr[1]) + '>: ' + data)
            reply = 'server output: ' + data
            if not data:
                break
            conn.sendall(str.encode(reply))
        except:
            pass
    print('closing client: ' + str(conn))
    conn.close()

# creating a socket object
s = socket.socket(socket.AF_INET,
                  socket.SOCK_STREAM)

# get local Host machine name
host = '' #socket.gethostname() # or just use (host == '')
port = 8800

# bind to pot
try:
    s.bind((host, port))
except socket.error as e:
    print(e)

# Que up to 5 requests
s.listen(5)
print("waiting for connections")
while True:
    conn, addr = s.accept()
    print('connected to: ' + addr[0] + ":" + str(addr[1]))

    start_new_thread(threaded_client,(conn,addr))