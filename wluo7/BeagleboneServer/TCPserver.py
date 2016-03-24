#!/usr/bin/env python

import sys
import re
import socket
import thread

from RequestHandlers.GEThandler import *

TCP_IP = '192.168.1.109'
TCP_PORT = 5005
BUFFER_SIZE = 1024
SERVER_ON = True 

# Parses the incoming request to appropriate function
def process_request(request):
	return get_data(request)	
	

# Called when connection is recieved
def handler(conn, addr):
	while True:
		request = conn.recv(BUFFER_SIZE)
		result = process_request(request)	
		conn.send(result)
		conn.close()
		print "Disconnected client ", addr
		break
					
if __name__ == '__main__':
# Initialize server socket
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind((TCP_IP, TCP_PORT))
	s.listen(3)

# Wait for client connection
	while SERVER_ON:
		conn, addr = s.accept()
		print "Connected to client ", addr
    		thread.start_new_thread(handler, (conn, addr))		
