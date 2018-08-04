import socket

ip = "127.0.0.1"
port = 8800

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((ip, port))
mesg = bytearray([1, 0])
print(mesg)
s.send(mesg)

