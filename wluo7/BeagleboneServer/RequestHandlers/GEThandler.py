#!/usr/bin/env python

import sys

# Retrieve most recent requested data from file
def get_data(data_request):
	try:
		f = open('../../SensorData/'+data_request+'.dat', 'r')
	except IOError:
		return "Invalid Request: "+'"'+data_request+'" data file not available.'

	# Returns the last (i.e. most recent) data line in data file
	line = f.readline()		
	while True:
		temp = f.readline()
		if not temp: break
		line = temp
	return line;
