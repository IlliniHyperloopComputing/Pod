# Basic code to interface the ADXL345 accelerometer to the BeagleBone Black
# 
# Hardware Setup: http://inspire.logicsupply.com/2014/08/beaglebone-3-axis-accelerometer.html
# 

#!/bin/python
from adxl345 import ADXL345
from time import sleep
 
adxl345 = ADXL345()
 
myfilename = "accel.dat"
with open(myfilename,'w') as f:
 print"ADXL345 on address 0x%x:" % (adxl345.address)
 f.write(str("#X,Y,Z ADXL345 accelerometer readings\n"))
 try:
  for i in range(1000):
   axes = adxl345.getAxes(True)
   xVal = axes['x']
   yVal = axes['y']
   zVal = axes['z']  
 
   print "x = %.3fG" % ( xVal )
   print "y = %.3fG" % ( yVal )
   print "z = %.3fG" % ( zVal )
 
   f.write(str(xVal) + ",")
   f.write(str(yVal) + ",")
   f.write(str(zVal) + '\n')
   sleep(.1) 
 except KeyboardInterrupt:
  exit()