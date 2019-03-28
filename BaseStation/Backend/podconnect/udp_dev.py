import socket
import time
UDP_IP = "127.0.0.1"
UDP_PORT = 5005
MESSAGE = "PING"

print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT

sock2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
sock2.bind((UDP_IP, 5004))

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP

sock2.settimeout(1.5) #if socket gets nothin in x seconds, throws an error

while True:
  sock.sendto(MESSAGE, (UDP_IP, 5005))
  try:
    data, addr = sock2.recvfrom(1024) # buffer size is 1024 bytes
    print "SENDERE: received message:", data
    time.sleep(.9)
  except:
    print "Never received message"
