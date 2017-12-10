import serial # you might need to: pip install PySerial
import sys
import select
import time
import datetime
import signal
import sys

ddata = False   # "Double data". A special case where everything recieved from
                # the arduinos (except force) is duplicated. 
                # this is designed for testing two motors at once
                # The plan is to split the data into two files, so the same processing
                # code can be used

if(len(sys.argv)>1 and sys.argv[1] == "2"):
    ddata = True

ctrl_c = 0
def signal_handler(signal, frame):
   global ctrl_c
   ctrl_c = 1
   print("GET SIGNALED BOI\n")
signal.signal(signal.SIGINT, signal_handler)

# Arduino settings RPM
serial_port_arduino = '/dev/ttyACM0' # you will probably need to change this
baud_rate_arduino = 115200

serial_port_arduino1 = '/dev/ttyACM1' 
baud_rate_arduino1 = 115200

serial_port_arduino2 = '/dev/ttyUSB1' 
baud_rate_arduino2 = 115200

# Omega settings
serial_port_omega = '/dev/ttyUSB0'
baud_rate_omega = 9600

output_txt_prefix = "raw_data/rpm_test_"

try:
    ser_arduino = serial.Serial(serial_port_arduino, baud_rate_arduino)
    ser_arduino1 = serial.Serial(serial_port_arduino1, baud_rate_arduino1)
    ser_arduino2 = serial.Serial(serial_port_arduino2, baud_rate_arduino2)
except:
    print("Failed to open arduino serial port, exiting")
    print("\tTry 'sudo'?")
    exit()

try:
    ser_omega = serial.Serial(serial_port_omega, baud_rate_omega)
except:
    print("Failed to open omega serial port, exiting")
    print("\tTry 'sudo'?")
    exit()


output_file_name = output_txt_prefix + datetime.datetime.now().isoformat('T')[:-7] #remove the last 7 characters, which are the fractions of a second
output_file = open(output_file_name, "w+")
output_file2 = ""
if(ddata):
    output_file2 = open(output_file_name+"_2", "w+")

print("Writing to file: %s" % output_file_name)
print("Begin reading ")
print("time, special, [RPM], [Thermo],  [Voltage, amps], force")

writing_delay = 8.0 #when initially reading serial ports in, there are transient errors

special = 8888
rpm_line    = ""
force_line  = ""
thermo_line = ""
power_line  = ""

new_data = False
start = time.time()
while (ctrl_c == 0):
    sl = select.select([sys.stdin, ser_arduino, ser_arduino1, ser_arduino2, ser_omega,], [], [], 0.0)[0]

    #read from stdin
    if (sys.stdin in sl):
        special = special+1 #sys.stdin.readline().rstrip()
        #print("Setting special to " + special)
        new_data = True

    #read from arduino RPM
    if (ser_arduino in sl): 
        rpm_line = ser_arduino.readline()
        rpm_line = rpm_line.decode("utf-8",errors="replace").replace('\n',' ').replace('\r','')
        new_data = True

    #read from arduino Thermo1
    if (ser_arduino1 in sl): 
        thermo_line = ser_arduino1.readline()
        thermo_line = thermo_line.decode("utf-8",errors="replace").replace('\n',' ').replace('\r','')
        new_data = True

    #read from arduino Power2
    if (ser_arduino2 in sl): 
        power_line = ser_arduino2.readline()
        power_line = power_line.decode("utf-8",errors="replace").replace('\n',' ').replace('\r','')
        new_data = True

    #read from omega
    if (ser_omega in sl): 
        force_line = ser_omega.readline()
        force_line = force_line.decode("utf-8").replace('\n',' ').replace('\r','')
        new_data = True


    if (new_data):
        stamp = time.time() - start
        output_string = ("%.4f %d %s %s %s %s\n" % (stamp, special, rpm_line, power_line, force_line, thermo_line))
        print("                                                                     ", end='\r')
        print(output_string.replace('\n',''), end='\r')

        if(stamp > writing_delay):
            if(not ddata): #Normal case
                output_file.write(output_string)
            else: #Case with duplicate data
                rpm_a   = rpm_line.split(' ')
                power_a = power_line.split(' ')
                thermo_a= thermo_line.split(' ')
                
                output_string_1 = ("%.4f %d %s %s %s %s\n" % (stamp, special, rpm_a[0], (power_a[0]+" "+power_a[1]), force_line, thermo_a[0]))
                output_string_2 = ("%.4f %d %s %s %s %s\n" % (stamp, special, rpm_a[1], (power_a[2]+" "+power_a[3]), force_line, thermo_a[1]))
                output_file.write(output_string_1)
                output_file2.write(output_string_2)

    new_data = False
ser_arduino.close()
ser_arduino1.close()
ser_omega.close()

