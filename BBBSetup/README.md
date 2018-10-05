This folder contains all of the setup information for the BBB. Device tree overlays, pinout information, kernel modules, and some setup scripts

# BBB OS Setup
  * Using this [Debian 9.4 image](http://debian.beagleboard.org/images/bone-debian-9.4-iot-armhf-2018-06-17-4gb.img.xz)
    * This guide is specific to just this version of Debian, with the 4.14 kernel. There have been big, breaking changes from previous kernel versions. uEnv.txt is differnt. Device overlays are different.
  * Setup network by editing `/etc/network/interfaces` and adding:
```
allow-hotplug eth0
iface eth0 inet static
  address 192.168.137.100
  netmask 255.255.255.0
  gateway 192.168.137.0
  network 129.168.137.1
```
  * I like having a static ethernet port so it is easy to SSH into. SSHing over USB is slower and usually cuts out for some reason
  * I also modified the config file for `connman`, the network service. I removed `wifi` from preferred devices list, and black listed it. I also black listed `can0`. the config file is `/etc/connman/main.conf`
  * Changes made to `boot/uEnv.txt`: (To enable our overlays that we will setup later)
```
#I uncommented, and then modified these lines to list the proper overlays
uboot_overlay_addr0=/lib/firmware/BB-ADC-00A0.dtbo
uboot_overlay_addr1=/lib/firmware/BB-DCAN1-00A0.dtbo
uboot_overlay_addr2=/lib/firmware/BB-PRU-01-00A0.dtbo
uboot_overlay_addr3=/lib/firmware/BB-PWM1-00A0.dtbo
uboot_overlay_addr4=/lib/firmware/w1-00A0.dtbo

#I uncommented these, such that video (HDMI) is disabled (its pins overlap with the PRU)
disable_uboot_overlay_video=1
disable_uboot_overlay_audio=1

#I commented this out, to disable the default "universal" cape
#enable_uboot_cape_universal=1
```

# Steps to get BBB configured:
1. Do the above to make sure the following will work
2. Run `copyAll` to move over all necessary files to BBB. Password is `temppwd`, which is the default for the user `debian`
3. `ssh` into the BBB using `debian@192.168.137.100` with password `temppwd`
4. Run `./setupOverlay`. Change the permissions of this file if necessary
5. Restart the BBB and check the `dmesg` for success/ errors of overlays
6. Run `./initPRU`. This must be run every time the BBB is turned on.

# BBB Overlay and PRU information:
* CAN Bus device tree overlay and kernel module setup
  * [Reference here](https://electronics.stackexchange.com/questions/195416/beaglebone-black-can-bus-setup)
  * Device tree _and_ kernel modules will be needed. Likely additional can utils will need to be installed.
  * Run `ifconfig` to see if can0 shows up. The device will need to be connected before anything will show up.
* ADC input device tree overlay setup
  * [Reference here](https://www.teachmemicro.com/beaglebone-black-adc/)
  * Look in the `/sys/bus/iio/devices/iio:device0` folder for ADC values
* PWM output device tree overlay setup
  * [Reference here](https://www.teachmemicro.com/beaglebone-black-pwm-ubuntu-device-tree/)
  * Look in the `/sys/class/pwm/pwmchip0/pwm0/` folder for the ability to write to PWM
  * The above was setup following instructions in the reference guide.
* GPIO input for PRU, for Optical inputs, device tree overlay setup
  * The main challenge with the PRU is that many of the guides were written several years ago (when the BBB came out) and since there have been updates to the kernel. (now on 4.14)
  * TI's [main page on the PRU-ICSS](http://processors.wiki.ti.com/index.php/PRU-ICSS).
  * Use this --> [TI's walk through guide](http://processors.wiki.ti.com/index.php/PRU_Training:_Hands-on_Labs). 
  * This is [pretty good guide too](http://processors.wiki.ti.com/index.php/RPMsg_Quick_Start_Guide#Booting_the_Board_and_Testing_RPMsg)
  * [elinux guide](https://elinux.org/EBC_Exercise_30_PRU_via_remoteproc_and_RPMsg) Look at the BeagleScope example for how someone else has done it.
  * [Best reference](https://elinux.org/images/d/da/Am335xPruReferenceGuide.pdf) I could find explaining all of the register level details of the PRU. 
  * The PRU Header files necessary for development with their IDE are included in this repo. They were found [here](http://www.ti.com/tool/PROCESSOR-SDK-AM335X). Pretty confusing, but it was in the "PROCESSOR\_SDK\_LINUX\_AM335X", once installed it was in some example files for the PRU.
  * _OLD_ An alternative method for [loading modules](https://groups.google.com/forum/#!topic/beagleboard-gsoc/amMNLt4EoHM), which links to this [gist](https://gist.github.com/alexanderhiam/2c4187c710b2c409d8dde8c4015fe007).
  * _OLD_ [Another developer blog with links](https://ianrrees.github.io/2016/11/20/getting-started-with-beaglebone-pru-programming-the-new-way.html)
  * _OLD_ guide on setting the PRUSS -- Programmable Real-time Unit Sub System. [Derek Molloy's site](http://exploringbeaglebone.com/chapter13/)
* GPIO output device tree overlay setup
  * [Derek Molloy's site](http://derekmolloy.ie/gpios-on-the-beaglebone-black-using-device-tree-overlays/). Goes through how to install the device tree, and most importently how to initialize the GPIO once it has been setup in the device tree.
  * Check `/sys/class/gpio/` 
  * [Device-Tree Overlay Generator](http://www.kilobaser.com/blog/2014-07-28-beaglebone-black-devicetreeoverlay-generator). 
* One-Wire communication protocol for DS18B20 temperature sensor device tree overlay
  * [Reference here](http://www.bonebrews.com/temperature-monitoring-with-the-ds18b20-on-a-beaglebone-black/).
  * Check the comments, there are syntax errors. There is a missing `;` on line 37.
  * Run `cat /sys/bus/w1/devices/28-*/w1_slave` to get the temperature, multiplied by 1000.
* I2C is already enabled by default by the kernel. On I2C2 bus.
* View all device trees on BBB: `dtc -f -I fs /proc/device-tree | less `
* [Device-Tree Overlay Generator](http://www.kilobaser.com/blog/2014-07-28-beaglebone-black-devicetreeoverlay-generator). 


# Pin reference
* The two PDFs in this directory contain detailed information about each header. 
* CAN Bus 
  * P9-24 (CAN RX) and P9-26 (CAN TX).
* ADC 
  * P9-33 through P9-40
* PWM
  * P9-14 (EHRPWM1A) or P9-16 (EHRPWM1B)
* GPIO input for PRU, Optical sensors
  * P8-27 through P8-29, and P8-39 through P8-46
* GPIO output 
  * P8-7 through P8-10
* DS18B20 temperature sensors
  * P8-11 (and optionally P8-12)
* I2C2
  * P9-19 and P9-20
* [default BBB pinout](beaglebone-black-pinout.jpg)

