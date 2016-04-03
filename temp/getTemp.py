# Basic code to interface the TMP36 temperature sensor to the BeagleBone Black
# 
# Hardware Setup:https://learn.adafruit.com/measuring-temperature-with-a-beaglebone-black?view=all
# 
import Adafruit_BBIO.ADC as ADC
import time
 
sensor_pin = 'P9_40'
 
ADC.setup()
 
while True:
    reading = ADC.read(sensor_pin)
    millivolts = reading * 1800  # 1.8V reference = 1800 mV
    temp_c = (millivolts - 500) / 10
    temp_f = (temp_c * 9/5) + 32
    print('mv=%d C=%d F=%d' % (millivolts, temp_c, temp_f))
    time.sleep(1)