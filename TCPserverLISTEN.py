import socket


TCP_IP = '127.0.0.1'
TCP_PORT = 5005
ME_TCP_IP = '127.0.0.1'
ME_TCP_PORT = 7008
BUFFER_SIZE = 20  # Normally 1024, but we want fast response

#s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s2.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s2.bind((ME_TCP_IP, ME_TCP_PORT))
s2.listen(1)

conn, addr = s2.accept()
print 'Connection address:', addr
while 1:
	data = conn.recv(BUFFER_SIZE)
	print "received data:", data
	conn.send("World ")
conn.close()