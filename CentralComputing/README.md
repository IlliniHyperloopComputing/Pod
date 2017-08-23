# Building the project using the Makefile

### There are two types of executables
* The "normal" type will build using whatever default version of g++ is installed on your system
  * Its prefix is `build`
* The "cross" type will build targeting the BeagleBone, using the `arm-linux-gnueabihf-g++` compiler and associated tool chain
  * Its prefix is `cross`

### There are three modes
* Release. This compiles with optimizations, and disables any `assert()` statements
  * No postfix 
* Debug. This compiles without optimizations, and defines `DEBUG`
  * `-debug` postfix
* Test. This is the same as Debug, but also defines `POD_TEST`, which runs a unit test suit instead of the normal behavior
  * `-test` postfix

### Examples:
* `make` will compile to "normal" in debug mode. 
  * Output: `build-debug`
* `make build` will compile to "normal" in release mode. 
  * Output: `build`
* `make build-test` will compile to "normal" in test mode. 
  * Output: `build-test`
* `make cross` will compile to "cross" in release mode. 
  * Output: `cross`
* `make cross-debug` will compile to "cross" in debug mode.
  * Output: `cross-debug`

### Other commands:
* `make clean` cleans up all executables and objects
* `make push` uses scp to copy all cross executables over to a Beaglebone

