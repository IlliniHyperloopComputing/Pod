This part of the guide assumes that the device trees are all appropriatly installed. 

The best technical guide will be the [Newest TI Reference.](http://processors.wiki.ti.com/index.php/Linux_Core_ADC_Users_Guide) However, the device tree was developed from the [older reference here.](https://www.teachmemicro.com/beaglebone-black-adc/) Although the installation steps are now wrong, the device tree should still be the same. 

## Running the test application

These are the development steps I took to figure out how to use the ADC properly.

The 'older refernce' had some source code for a test executable that could setup and read from the ADCs streaming buffer. This is implemented through `generic_buffer.c`, which the site says was in kernel source at: `drivers/staging/iio/Documentation/`. They also provide a patch that removes hardware triggering. I could not find the origin of these files, but I did find this version of [generic_buffer.c](https://github.com/torvalds/linux/blob/6f0d349d922ba44e4348a17a78ea51b7135965b1/tools/iio/iio_generic_buffer.c), which seems to match pretty closely. Moral of the story: no idea where the site's `generic_buffer.c` and the patch came from.
  * `generic_buffer.c` contains the correct patched version
  * `generic_buffer.c.orig` contains the unpatched version 
  * `generic_buffer.patch` is the patch file

The old site also uses the following kernal files, which are also included in this directory.
  * [iio_utils.c](https://github.com/torvalds/linux/blob/master/tools/iio/iio_utils.c)
  * [iio_utils.h](https://github.com/torvalds/linux/blob/master/tools/iio/iio_utils.h)

The executable should be able to be compiled with:
  * `gcc --static generic_buffer.c iio_utils.c -o generic_buffer`
And run with:
  * `echo 1 > /sys/bus/iio/devices/iio\:device0/scan_elements/in_voltageX_en` where X can be `0` through `6`. Setting multiple of these to `1` enables multiple channels to stream at once.
  * `echo 100 > /sys/bus/iio/devices/iio\:device0/buffer/length` sets buffer length. Although I believe the following executable overwrites this value, it seems to be needed to set on a fresh restart.
  * `echo 0 > /sys/bus/iio/devices/iio\:device0/buffer/enable` will disable the buffer. If this is set to `1` the following executable will not work. This is because it will try to modify `buffer/length` which fails if `enable` is set to `1`.
  * `sudo ./generic_buffer -n TI-am335x-adc -l 5 -c 3` where `l` specifies buffer size, and `c` specifies number of iterations

## Development Progress
  * `simplified_buffer.c` is a stripped down version of `generic_buffer.c`, or maybe you could call it a specific buffer. This was done to increase my understanding as to what was actually going on.
  * Now development is shifting to the CentralComputing library


## NEW untested approach

Upon setting up this README and putting it onto git I realize that there is a new TI reference page (I had initially found the deprecated version), so now all the TI links are correct. At the bottom of their page they list a method to create a similar `generic_buffer` executable using the new kernal example source code. These instructions are at the bottom of the [newest TI reference.](http://processors.wiki.ti.com/index.php/Linux_Core_ADC_Users_Guide)
