This folder contains all of the device tree overlays, kernal modules, and a script to set everything up. All of the source files must be copied over to the BBB, of course.

# BBB setup includes: 
* CAN Bus device tree overlay and kernal module setup
  * [Reference here](https://electronics.stackexchange.com/questions/195416/beaglebone-black-can-bus-setup)
  * Device tree _and_ kernal modules will be needed. Likely additional can utils will need to be installed.
  * Run `ifconfig` to see if can0 shows up. The device will need to be connected before anything will show up.
* ADC input device tree overlay setup
  * [Reference here](https://www.teachmemicro.com/beaglebone-black-adc/)
  * Look in the `/sys/bus/iio/devices/iio:device0` folder for ADC values
* PWM output device tree overlay setup
  * [Reference here](https://www.teachmemicro.com/beaglebone-black-pwm-ubuntu-device-tree/)
  * Look in the `/sys/class/pwm/pwmchip0/pwm0/` folder for the ability to write to PWM
  * The above was setup following instructions in the reference guide.
* GPIO input for PRU, for Optical inputs, device tree overlay setup
  * [Device-Tree Overlay Generator](http://www.kilobaser.com/blog/2014-07-28-beaglebone-black-devicetreeoverlay-generator). 
* GPIO output device tree overlay setup
  * [Device-Tree Overlay Generator](http://www.kilobaser.com/blog/2014-07-28-beaglebone-black-devicetreeoverlay-generator). 
* One-Wire communication protocol for DS18B20 temperature sensor device tree overlay
  * [Reference here](http://www.bonebrews.com/temperature-monitoring-with-the-ds18b20-on-a-beaglebone-black/).
  * Check the comments, there are syntax errors. There is a missing `;` on line 37.
  * Look in `/sys/devices/` for the sensors. They must be plugged in before anything will show up.

# How to apply settings
1. Copy over all `.dts` files to the BBB using `scp`
2. Copy over the `initOverlay` script to the BBB   
3. Run the `./initOverlay` script. This must be in the same directory as the `.dts` files. Change the permissions of this file if necessary


