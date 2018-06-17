#!/bin/sh

# Might need to: `sudo apt-get install linux-image-extra-virtual`
# in reference to the above: https://askubuntu.com/questions/541443/how-to-install-usbserial-and-ftdi-sio-modules-to-14-04-trusty-vagrant-box


sudo modprobe cp210x
sudo sh -c 'echo 10c4 867d > /sys/bus/usb-serial/drivers/cp210x/new_id'

# A good reference for why we do the above
# https://stackoverflow.com/questions/38330155/updating-product-vendor-id-on-raspberry-pi-cp210x


#Then, once the usb is plugged in and assinged a location: /dev/ttyUSB[n]
#To set 9600 baud, no parity, 8 bits, 1 stop bit: 
# `stty -F /dev/ttyUSB[n] 9600 cs8 -cst0pb -parenb` 






