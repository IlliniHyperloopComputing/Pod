#!/usr/bin/env python

import sys

# Retrieve most recent requested data from file
def get_data(data_request):
	try:
		location = '../SensorData/'+data_request+'.dat'
		f = open(location, 'r')
	except IOError:
		return "Invalid Request: "+'"'+location+'" data file not available.'

	# Finds the last (i.e. most recent) data line in data file
	line = f.readline()		
	while True:
		temp = f.readline()
		if not temp: break
		line = temp

	# Return the most recent data line from file
	return line;
