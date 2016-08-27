import re

f = open('accelerometer.dat', 'r')

line = f.readline()
while True:
	temp = f.readline()
	if not temp: break	
	line = temp

match = re.search(r'(\S+):(\S+)\s(\S+)', line)
print match.group(1)
print match.group(2)
print match.group(3)
