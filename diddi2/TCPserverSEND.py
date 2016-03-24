import socket 
   
TCP_IP = '127.0.0.1'
TCP_PORT = 7008
BUFFER_SIZE = 1024
ME_TCP_IP = '127.0.0.1'
ME_TCP_PORT = 5005
MESSAGE = "Hello "

s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s1.bind((ME_TCP_IP, ME_TCP_PORT))
s1.listen(1)
 
s2.connect((TCP_IP, TCP_PORT))
s2.send(MESSAGE)

while 1:
   data = s2.recv(BUFFER_SIZE)
   print data
   s2.send(data)

conn, addr = s1.accept()
print 'Connection address:', addr
while 1:
    data2 = conn.recv(BUFFER_SIZE)
    if not data2: break
    print "received data:", data2
    conn.send(MESSAGE)
    conn.send(MESSAGE)  # echo
conn.close()

s1.close()
s2.close()  
print "received data:", data 