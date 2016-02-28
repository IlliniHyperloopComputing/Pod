import socket
import json as js

UDP_IP = "127.0.0.1"
UDP_PORT = 5005
SENDER_UDP_IP = "127.0.0.1"
SENDER_UDP_PORT = 8008
BUFFER_SIZE = 1024
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind((UDP_IP, UDP_PORT))
count = 0
while 1:
	data, addr = sock.recvfrom(BUFFER_SIZE) 
	if data:
		count += 1
		print "received message:", js.loads(data)
		sock.sendto(js.dumps(count), (SENDER_UDP_IP, SENDER_UDP_PORT))
sock.close();	