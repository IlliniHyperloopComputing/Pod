import socket, time

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(("127.0.0.1", 8800))
print("Connected")
time.sleep(1)
while(True):
  buffer = client.recv(1)
  print(buffer)



