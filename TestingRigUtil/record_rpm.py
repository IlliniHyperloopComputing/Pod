import serial
import sys
import select
import time
import datetime

# you might need to: pip install PySerial

serial_port = '/dev/ttyACM0' # you will probably need to change this
baud_rate = 9600
output_txt_prefix = "rpm_test_"
power = "899"

try:
    ser = serial.Serial(serial_port, baud_rate)
except:
    print("Opening serial port failed, exiting")
    exit()

output_file_name = output_txt_prefix + datetime.datetime.now().isoformat('T')
output_file = open(output_file_name, "w+")
print("Writing to file: %s" % output_file_name)

print("Begin reading")
start = time.time()
while True:
  if select.select([sys.stdin,], [], [], 0.0)[0]:
    power = sys.stdin.readline().rstrip()
    print("Setting power to " + power)
  else:
    line = ser.readline()
    line = line.decode("utf-8")
    stamp = time.time() - start
    output_string = ("%.4f" % stamp) + " " + power + "      " + line 
    output_file.write(output_string)
    print(output_string.replace('\n',''), end='\r')
    

