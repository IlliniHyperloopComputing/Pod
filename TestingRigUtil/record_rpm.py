import serial # you might need to: pip install PySerial
import sys
import select
import time
import datetime
import signal
import sys

ctrl_c = 0
def signal_handler(signal, frame):
   ctrl_c = 1 
signal.signal(signal.SIGINT, signal_handler)

# Arduino settings
serial_port_arduino = '/dev/ttyACM0' # you will probably need to change this
baud_rate_arduino = 9600

# Omega settings
serial_port_omega = '/dev/ttyUSB0'
baud_rate_omega = 9600

output_txt_prefix = "rpm_test_"

try:
    ser_arduino = serial.Serial(serial_port_arduino, baud_rate_arduino)
except:
    print("Failed to open arduino serial port, exiting")
    exit()

try:
    ser_omega = serial.Serial(serial_port_omega, baud_rate_omega)
except:
    print("Failed to open omega serial port, exiting")
    exit()


output_file_name = output_txt_prefix + datetime.datetime.now().isoformat('T')
output_file = open(output_file_name, "w+")
print("Writing to file: %s" % output_file_name)
print("Begin reading")

power = "899"
a_line = ""
o_line = ""

start = time.time()
while (ctrl_c == 0):
    sl = select.select([sys.stdin, ser_arduino, ser_omega,], [], [], 0.0)[0]

    #read from stdin
    if (sys.stdin in sl):
        power = sys.stdin.readline().rstrip()
        print("Setting power to " + power)

    #read from arduino
    if (ser_arduino in sl): 
        a_line = ser_arduino.readline().replace('\n',' ').replace('\r','')
        a_line = line.decode("utf-8")

    #read from omega
    if (ser_omega in sl): 
        o_line = ser_omega.readline().replace('\n',' ').replace('\r','')
        o_line = line.decode("utf-8")


    stamp = time.time() - start
    output_string = ("%.4f %d  %s  %s \n" % stamp, power, a_line, o_line) 
    output_file.write(output_string)
    print(output_string.replace('\n',''), end='\r')
    

