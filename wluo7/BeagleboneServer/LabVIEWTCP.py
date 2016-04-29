#!/usr/bin/env python

import signal, os
import sys
import re
import socket
import thread

from RequestHandlers.GEThandler import *

TCP_IP = '192.168.137.10' # Change to 192.168.1.109 if on Beaglebone or its static ip
TCP_PORT = 5005
BUFFER_SIZE = 1024
SERVER_ON = True 

# Parses the incoming request to appropriate function
def process_request(request):
	return_list = []
	requests = request.split("\n")
	for x in requests:
		match = re.search(r'(\S+)\s(\S+)', x)	
		if match :
			type = match.group(1);
			content = match.group(2)
			
			if(content == "TMP" or content == "BAT"):
				continue
			
			# Include corresponding functions for avaiable http requests
			switch = {
				'GET': get_data,
			}
			try:
				return_list.append (switch.get(type)(content))
			except TypeError, NameError:
				return 'Invalid request type "'+type+'"'	
		else:
			return return_list
			#return 'Invalid, can not parse request "'+request+'"'	
	return return_list


# Called when connection is recieved
def request_handler(conn, addr):
	while True:
		request = conn.recv(BUFFER_SIZE)
		#print "request %s" % (request)
		result = process_request(request)	
		#print "result: %s" % (result)
		if not isinstance(result, str):
			for x in result:
				conn.send(x)
		else:
			print "ERROR: %s" % result
			conn.close()
			print "connection closed with: ", addr
					
if __name__ == '__main__':
# Initialize server socket
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	s.bind((TCP_IP, TCP_PORT))
	s.listen(3)

# Wait for client connection
	try:
		while SERVER_ON:
			conn, addr = s.accept()
			print "Connected to client ", addr
			thread.start_new_thread(request_handler, (conn, addr))
	# Close server socket if ctrl-c is recieved
	except KeyboardInterrupt:
		s.close()
		print "\rBeagleboneServer socket at "+TCP_IP+":"+str(TCP_PORT)+" closed."
