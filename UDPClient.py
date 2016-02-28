import socket
import json as js

UDP_IP = "127.0.0.1"
UDP_PORT = 5005
BUFFER_SIZE = 1024
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while 1:
    data, addr = sock.recvfrom(BUFFER_SIZE) 
    print "received message:", js.loads(data)