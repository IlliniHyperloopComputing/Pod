#!/usr/bin/env python

import json
import socket
import time
import re

TCP_IP = '127.0.0.1' # Change to 192.168.1.109 if on Beaglebone or its ip
TCP_PORT = 5005
BUFFER_SIZE = 1024
MESSAGE = "GET gyroscope"

def sent_request(request):

	# Connect to server
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
	s.connect((TCP_IP, TCP_PORT));

	# Sent request
	s.send(request);

	# Retrieve result
	data = s.recv(BUFFER_SIZE)

	match = re.search(r'(\S+),(\S+)', data)
	if match:
		time = match.group(1)
		value = match.group(2)
	else:
		time = 0
		value = 0
	return json.dumps([time, value])
