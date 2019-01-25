This folder contains all of the setup information for the BBB. Device tree overlays, pinout information, kernel modules, and some setup scripts

# BBB OS Setup
  * Using this [Debian 9.4 image](http://debian.beagleboard.org/images/bone-debian-9.4-iot-armhf-2018-06-17-4gb.img.xz), follow the SD card installation steps at the bottom of this README.
    * This guide is specific to just this version of Debian, with the 4.14 kernel. There have been big, breaking changes from previous kernel versions. uEnv.txt is differnt. Device overlays are different.
  * To connect when plugged directly into your personal computer, I like having a static ethernet port. SSHing over USB is slower, and sometimes drops
    * Setup network by editing `/etc/network/interfaces` and adding:
  ```
  allow-hotplug eth0
  iface eth0 inet static
    address 192.168.137.100
    netmask 255.255.255.0
    gateway 192.168.137.0
    network 129.168.137.1
  ```
  * I also modified the config file for `connman`, the network service. I removed `wifi` from preferred devices list, and black listed it. I also black listed `can0`. the config file is `/etc/connman/main.conf`. Connman is described more [here](https://www.systutorials.com/docs/linux/man/5-connman.conf/)
  * To further disable 'extra' jobs from running in the background:
    * Disabled the apache2 webserver from starting up: `sudo systemctl disable apache2.service`
    * Remove NodeJs entirely: `sudo apt-get remove c9-core-installer` then `sudo apt-get purge nodejs`
    * Stop `wpa_supplicant` since we only use ethernet: `sudo systemctl disable wpa_supplicant.service`
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

#For PRUSS options, make sure only the following line is uncommented:
###pru_rproc (4.14.x-ti kernel)
uboot_overlay_pru=/lib/firmware/AM335X-PRU-RPROC-4-14-TI-00A0.dtbo
#If not setup correctly, PRU devices to not show up, ./initPRU will not work


#I commented this out, to disable the default "universal" cape
#enable_uboot_cape_universal=1
```

# Steps to get BBB configured:
1. Do the above to make sure the following will work
2. Run `copyAll` to move over all necessary files to BBB. Password is `temppwd`, which is the default for the user `debian`
3. `ssh` into the BBB using `debian@192.168.137.100` with password `temppwd`
4. Run `sudo ./setupOverlay`. Change the permissions of this file if necessary
5. Restart the BBB and check the `dmesg` for success/ errors of overlays
6. Run `sudo ./initPRU` to load the PRU firmware, and `sudo ./initCAN` to start the CAN interface, and `./initGPIO` to 'export' the gpio pins (any ordering works). **This must be run every time the BBB is restarted**

# BBB Overlay and PRU information:
* CAN Bus device tree overlay and kernel module setup
  * [Newest TI Reference](http://processors.wiki.ti.com/index.php/Linux_Core_ADC_Users_Guide)
  * [Deprecated TI Reference here](https://electronics.stackexchange.com/questions/195416/beaglebone-black-can-bus-setup)
  * Device tree _and_ kernel modules will be needed. Likely additional can utils will need to be installed.
  * Run `ifconfig` to see if can0 shows up. The device will need to be connected before anything will show up.
* ADC input device tree overlay setup
  * [TI Refernce here](http://processors.wiki.ti.com/index.php/AM335x_ADC_Driver%27s_Guide)
  * More information within `adc_testing` subdirectory
  * [older reference here](https://www.teachmemicro.com/beaglebone-black-adc/). It has some source code for a test executable. More information in `adc_testing` subdirectory.
  * [another technical reference](http://beaglebone.cameon.net/home/reading-the-analog-inputs-adc)
  * Look in the `/sys/bus/iio/devices/iio:device0` folder for ADC values
* PWM output device tree overlay setup
  * [Reference here](https://www.teachmemicro.com/beaglebone-black-pwm-ubuntu-device-tree/)
  * Look in the `/sys/class/pwm/pwmchip0/pwm0/` folder for the ability to write to PWM
  * The above was setup following instructions in the reference guide.
* PRU usage for Optical inputs, device tree overlay setup
  * The main challenge with the PRU is that many of the guides were written several years ago (when the BBB came out) and since there have been updates to the kernel. (now on 4.14)
  * Use this --> [TI's walk through guide](http://processors.wiki.ti.com/index.php/PRU_Training:_Hands-on_Labs). 
  * TI's [main page on the PRU-ICSS](http://processors.wiki.ti.com/index.php/PRU-ICSS).
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
  * 'export' a GPIO by: `echo <num> /sys/class/gpio/export`, where you replae `<num>` with the appropriate "GPIO No." fouund in the P8 and P9 HeaderTable.pdf 
  * Change to output or input with `echo "out" > /sys/class/gpio/gpio<num>/output`
  * Set pin value with `echo 1 > /sys/class/gpio/gpio<num>/value`, or read the file if an input
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


# BBB SD Card installation

This guide covers how to setup a BeagleBone Black Rev.c with all necessary software 

* Download the appropriate image as specified earlier in this document. Although it is dated, I have found [this](http://derekmolloy.ie/write-a-new-image-to-the-beaglebone-black/) to generally be a good guide, but is slightly outdated (as explained below)
	* The old guide assumes that the image will automatically begin writing itself to the onboard storage. As described on the [beagleboard.org](https://beagleboard.org/latest-images) site, you will need to modify the `/boot/uEnv.txt` file
	* The correct modifications are made later in this guide

* Copy the `.img` file on to a microSD card
	* Windows 
		* Extract the `.img` using WinRAR
		* Copy onto the uSD card using Win32 Disk imager
	* Linux 
		* Extract the `.img` using `xz -d bone-*.img.xz`  
		* Use `lsblk` to find the name of the microSD card
		* Make sure the microSD card is unmounted (not ejected)
		* Copy onto the uSD card using `sudo dd if=./bone-*.img of=/dev/NAME status=progress`
	* Mac 
		* Extract the `.img` using `gunzip bone-*.img.xz` 
		* Use `diskutil list` to find the name of the microSD card
		* Make sure the microSD card is unmounted (not ejected) using `diskutil unmountDisk /dev/disk#`
		* Copy onto the uSD card using `sudo dd bs=1m if=./bone-*.img of=/dev/rdisk# conv=sync status=progress`. If this fails, try `/dev/disk#` instead of `/dev/rdisk#`

* Insert the microSD card into the BBB, with the power off

* Hold down the boot button (near the SD card), and plug in power. 
	* I used USB power from my computer, such that I can SSH into the BBB 

* Hold the boot button until the LEDs flash

* Wait for the BBB to turn on. It has now booted onto the image on the SD card. 

* SSH into the BBB (I found the default username/password [here](http://elinux.org/BeagleBoardDebian))
	* The BBB has default drivers that setup a network connection over USB at IPs `192.168.7.2` and `192.168.6.2`. 
	* Windows
		* Wait until the BBB file dialog opens, then you will be able to SSH in. 
		* Use PuTTY or other appropriate terminal: `ssh debian@192.168.7.2` with the password `temppwd`. 
	* Linux (Ubuntu)
		* Check network connections with `ifconfig`
		* Use `ssh debian@192.168.7.2` with the password `temppwd`

* Once logged in
	* Edit `/boot/uEnv.txt` use: `sudo vim /boot/uEnv.txt` or `sudo nano /boot/uEnv.txt` 
	* Go to the bottom of the file, and remove the ‘#’ in front of `cmdline=init=/opt/scripts/tools/eMMC/init-eMMC-flasher-v3.sh` to uncomment it. 

* Reboot the BBB with `sudo shutdown -r now`
* Begin to hold the boot button
* Hold the boot button until the BBB powercycles, and the LEDs turn back on (just as above)
* After a few moments of booting up and LEDs flashing, the installation will start. The LEDs will flash in a Cyclon/ Knight Rider patter. 
* Wait until the LEDs stop flashing,
* The BBB should power off, but I have found that sometimes it finishes with all LEDs on. 
* Unplug the SD card, otherwise the image will re-install!
* Power back on the BBB! The newest image should be installed and ready to go
* SSH as described above
* Create a new user, named `abc`, with password `hyperloop`. Use: `sudo adduser abc`.
  * Add the new user to all of the groups that the user `debian` is a part of. This may not be strictly necessary, but I didn't want to spend the time to figure it out. List all the groups (while logged in as `debian`) with `groups`. 
  * Add the new user to each group with: `sudo usermod -aG <group_name> <user_name>`. At a minimum, add the user to the `sudo` group.


