import socket, time

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(("127.0.0.1", 8800))
print("Connected")
time.sleep(1)
for _ in range(0, 30):
  buffer = client.recv(1)
  print(buffer)

client.close()



