#!/usr/bin/python

import smbus
from Adafruit_I2C import Adafruit_I2C

class ADS1115_Read:

#This class will be used to read
#single convesions from the ADC
#===============
#The ADS1115 16-bit config register
#Bit[15] used to start a conversion
##Write: 
### 0 -> no effect
### 1 -> begin single conversion
##Read:
### 0 -> device is currently converting
### 1 -> device is not converting(finished)
#Bit[14:12] Which pin to use as input
##AIN = AIN0 -> 100
##AIN = AIN1 -> 101
##AIN = AIN2 -> 110
##AIN = AIN3 -> 111
#Bit[11:9] -> set gain
##FS=6.144v -> 000
##FS=4.096v -> 001
##FS=2.048v -> 010 (Default)
##FS=1.024v -> 011
##FS=0.512v -> 100
##FS=0.256v -> 101
#Bit[8] Device operating mode
## continuous -> 0
##single-shot -> 1 (Default)
#Bit[7:5] Data rate
##This is how long a conversion takes
##8SPS -> 000
##16SPS -> 001
##32SPS -> 010
##64SPS -> 011 (default
##128SPS -> 100
##250SPS -> 101
##475SPS -> 110
##860SPS -> 111
#Bit[4] comparator mode
##We wont use this
##Traditional comparator with hysteresis -> 0 (default)
#Bit[3] Comparator polarity
##again, won't be using this
## Active low -> 0 (default)
#Bit[2] Latiching comparator
##Won't be using this
## Non-latching comparator -> 0 (default)
#Bit[1:0]
##won't be using the comparator
##disable comparator -> 11 (default)
#===============

  i2c = None
  ADS_ADDRESS = 0x48  #default address
  ADS_POINTER_CONVERT= 0x00
  ADS_POINTER_CONFIG= 0x01
  
  START_SINGLE_CONVERSION= 0x8000;
  DEVICE_MODE_SINGLE = 0x100;
  DEVICE_MODE_CONTINUOUS = 0x000;
  LAST_5_BITS = 0x03;#bits[4:0] that we are not planning on changing

  # Dictionaries
  ain= {
    0:0x4000,
    1:0x5000,
    2:0x6000,
    3:0x7000
  }
  data_rate= {
    8:0x0000,
    16:0x0020,
    32:0x0040,
    64:0x0060,
    128:0x0080,
    250:0x00A0,
    475:0x00C0,
    860:0x00E0
  }    
  gain= {
    6144:0x000,
    4096:0x0200,
    2048:0x0400,
    1024:0x0600,
    512:0x0800,
    256:0x0A00
  }    

  def __init__(self, address=0x48):
    self.i2c = Adafruit_I2C(address)
    ADS_ADDRESS = address #default address
  
  def read(self, ain= 0, gain = 6144, data_rate =250):
  #No error checking because your smart
    config = self.START_SINGLE_CONVERSION | self. DEVICE_MODE_SINGLE | self.ain.get(ain) | self.data_rate.get(data_rate)| self.gain.get(gain) | self.LAST_5_BITS

    bytes = [(config >> 8) & 0xFF, config & 0xFF]#bytes to write

    self.i2c.writeList(self.ADS_POINTER_CONFIG, bytes)#write
    
    read_buff = [0,0]
    while((read_buff[0] & 0x80) == 0):
      read_buff= self.i2c.readList(self.ADS_POINTER_CONFIG, 2)
    
    result = self.i2c.readList(self.ADS_POINTER_CONVERT, 2)
    val = result[0]<<8 | result[1]
    # the proper value has 
    val = val * (gain/1000.0) / 32767.0
    return val
