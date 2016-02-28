import socket
import json as js
from time import time
from random import random
from flask import Flask, render_template, make_response

UDP_IP = "127.0.0.1"
UDP_PORT = 5005
SENDER_UDP_IP = "127.0.0.1"
SENDER_UDP_PORT = 8008
BUFFER_SIZE = 1024

app = Flask(__name__)


@app.route('/')
def hello_world():
    return render_template('index.html', data='test')


@app.route('/live-data')
def live_data():
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock.bind((UDP_IP, UDP_PORT))
	count = 0
	data, addr = sock.recvfrom(BUFFER_SIZE) 
	if data:
		print "received message:", js.loads(data)
		sock.sendto(js.dumps(count), (SENDER_UDP_IP, SENDER_UDP_PORT))
	#sock.close();	
	data = js.loads(data)
	data = data[2]
	data2 = [time() * 1000, data]
	response = make_response(js.dumps(data2))
	response.content_type = 'application/json'
	return response

if __name__ == '__main__':
    app.run(debug=True, host='127.0.0.1', port=5000)
