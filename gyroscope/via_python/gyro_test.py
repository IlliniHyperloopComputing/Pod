# Follow this guide on the adafruit_i2c module if this test code 
## is not running.
## https://learn.adafruit.com/setting-up-io-python-library-on-beaglebone-black/i2c
## Probably, you need to apt-get python-smbus
##
from Adafruit_I2C import Adafruit_I2C
import time
address = 0x6b #I2C address. can also be 0x6a if SD0 is grounded
i2c = Adafruit_I2C(address)

print "id: %d" % i2c.readU8(0x0F) #print device id. 

i2c.write8(0x20,0x0F) #switch to normal mode(enable x,y,z) (set Hz to 100)
i2c.write8(0x23,0x00) #set resolution. 245 dps, sensitivity = 0.00875

xrot = 0#the actual rotation in each axis
yrot = 0
zrot = 0

xlo = 0
xhi = 0
xdata = 0
ylo = 0
yhi = 0
ydata = 0
zlo = 0
zhi = 0
zdata = 0

xavg = 0
yavg = 0
zavg = 0

check_bit = 0

###calibrate
n=0
c = 1000
while n<c:
  check_bit = i2c.readU8(0x27) 
  if(check_bit & 8 == 8):
    xlo = i2c.readU8(0x28)
    xhi = i2c.readS8(0x29)
    ylo = i2c.readU8(0x2A)
    yhi = i2c.readS8(0x2B)
    zlo = i2c.readU8(0x2C)
    zhi = i2c.readS8(0x2D)
    xavg += (xlo | (xhi<<8)) * 0.0000875
    yavg += (ylo | (yhi<<8)) * 0.0000875
    zavg += (zlo | (zhi<<8)) * 0.0000875
    n += 1
  
xavg = xavg/c
yavg = yavg/c
zavg = zavg/c
print "xavg %f\tyavg %f\tzavg %f" % (xavg,yavg,zavg)


n=0
while n<2000:
  check_bit = i2c.readU8(0x27) 
  if(check_bit & 8 ==8):

    xlo = i2c.readU8(0x28)#read the xlo bit. it is signed
    xhi = i2c.readS8(0x29)#read the xhi bit. it is signed
    ylo = i2c.readU8(0x2A)
    yhi = i2c.readS8(0x2B)
    zlo = i2c.readU8(0x2C)
    zhi = i2c.readS8(0x2D)
    xdata = (xlo | (xhi<<8)) #- xavg
    ydata = (ylo | (yhi<<8)) #- yavg
    zdata = (zlo | (zhi<<8)) #- zavg

    xdata *= 0.00875
    ydata *= 0.00875
    zdata *= 0.00875

    xrot += xdata * 0.01 - xavg # multiply by frequency (default is 100 hz)
    yrot += ydata * 0.01 - yavg
    zrot += zdata * 0.01 - zavg
    n = n+1

print "x %f\ty %f\tz %f" % (xrot,yrot,zrot)
