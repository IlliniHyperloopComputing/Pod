import random as r
import socket
import json as js
import time

UDP_IP = "127.0.0.1"
UDP_PORT = 5005
ME_UDP_IP = "127.0.0.1"
ME_UDP_PORT = 8008
BUFFER_SIZE = 1024
NUM_OF_SENSORS = 5
SLEEP_SECONDS = 1.0
ON = 1
OFF = 0

STOPPER_COND = 20

def generateRandomData(sensorTotal):
	return [ r.randint(1,10*(i+1)) for i in range(sensorTotal)]

def updateData(generatedData):
	return
	#Update Global which has current data State. Does Nothing Right now

def sendToClients(generatedData):
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock.sendto(js.dumps(generatedData), (UDP_IP, UDP_PORT))


stop = ON
stopper = js.dumps(STOPPER_COND)
while stop != OFF :	
	time.sleep(SLEEP_SECONDS)
	listener = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
	listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	listener.bind((ME_UDP_IP, ME_UDP_PORT))
	generatedData = generateRandomData(NUM_OF_SENSORS)
	print (generatedData)
	updateData(generatedData)
	sendToClients(generatedData)
	#data, addr = listener.recvfrom(BUFFER_SIZE)
	#print js.loads(data)
	#if (js.loads(data) == js.loads(stopper)): 
	#	stop = OFF
