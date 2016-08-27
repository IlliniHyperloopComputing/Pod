#!/usr/bin/python
import signal, sys
from adc_read import ADS1115_Read

def signal_handler(signal, frame):
        print 'You pressed Ctrl+C!'
        sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)

gain = 4096
sps = 860


adc = ADS1115_Read()

count = 0;
while count < 20000:
  distance = adc.read(3)
  distance = 12.434 * ( distance ** (-1.065))#approximation into cm
  xaccel = adc.read(0)#accelerometer also hooked up
  yaccel = adc.read(1)
  zaccel = adc.read(2)
  print "Dist: %.4f\tx: %.4f\ty: %.4f\tz: %.4f" % (distance,xaccel,yaccel,zaccel)
  count += 1



