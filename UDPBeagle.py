import random as r
import socket
import json as js

UDP_IP = "127.0.0.1"
UDP_PORT = 5005
MESSAGE = "Hello, World!"


def generateRandomData(sensorTotal):
	return [ r.randint(0,10*i) for i in range(sensorTotal)]

def updateData(generatedData):
	return
	#Update Global which has current data State

def sendToClients(generatedData):
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.sendto(js.dumps(generatedData), (UDP_IP, UDP_PORT))
	sock.close();

for i in range(100):
	generatedData = generateRandomData(5)
	print (generatedData)
	updateData(generatedData)
	sendToClients(generatedData)