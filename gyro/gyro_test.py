from Adafruit_I2C import Adafruit_I2C
import time
address = 0x6b #I2C address. can also be 0x6a if SD0 is grounded
i2c = Adafruit_I2C(address)

print "id: %d" % i2c.readU8(0x0F) #print device id. 

i2c.write8(0x20,0x0F) #switch to normal mode
i2c.write8(0x23,0x00) #set resolution. 

time.sleep(1)

n=0
while n<20000:
	time.sleep(.05)

	xlo = i2c.readU8(0x28)#read the xlo bit. it is signed
	xhi = i2c.readS8(0x29)#read the xhi bit. it is signed
	print ((xlo | (xhi <<8)) * 0.00875);# bit shift into place and multiply

	n = n+1






