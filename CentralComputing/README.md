# Building the project using the Makefile

### There are two types of executables
* The `build` uses whatever default version of `g++` is installed on your system
* The `cross` uses the `arm-linux-gnueabihf-g++` compiler and associated tool chain, so it can be compiled on a host machine and copied over to the 32-bit ARM Beaglebone Black

### There are three modes
* Release: 
  * defines `NDEBUG`, and compiles with optimizations
  * `build` and `cross`
* Debug:
  * defines `DEBUG`, and compiles without optimizations
  * `dbuild` and `dcross`
* Simulation:
  * defines `DEBUG` and `SIM`, and compiles without optimizations
  * `sbuild` and `scross`

### Examples:
* `make` will compile two things:
  * `dbuild` and `sbuild`
* `make [exe name]` will compile a specific executable
  * `build` `dbuild` `sbuild` `cross` `dcross` `scross` are valid options

### Other commands:
* `make clean` cleans up all executables and objects
* `make push` uses scp to copy all `cross` executables over to a Beaglebone

