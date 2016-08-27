#set the date and time
/usr/bin/ntpdate -b -s -u pool.ntp.org
#install dependency
opkg update && opkg install python-pip python-setuptools python-smbus
pip install Adafruit_BBIO