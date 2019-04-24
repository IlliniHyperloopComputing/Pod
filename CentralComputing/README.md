# Overview of code structure 
The code is designed to run on a Linux platform, specifically on the BeagleBone Black (BBB). More detail on BBB setup can be found in the `BBBSetup/` folder in this repository.

The Pod software runs several threads to handle multiple different IO streams. Although the BBB is a single-core computer, multiple threads make scheduling simpler. Sleeping, blocking IO, waiting on data, are all very to do with multiple threads. In fact, most threads have very little computational overhead, and are bottlenecked by access to external IO resources. 

### The following resources are accessed:
  * ADC Built-in to the BBB 
  * I2C Port, via pins on the BBB
  * CANBus, via UARTs pins on the BBB
  * Ethernet, TCP and UDP 
  * PRU (Programmable Real-time Unit) Built-in to the BBB. See `BBBSetup/` for more.

### State Machine
A state machine is used to easily define Pod behavior. Specifically the `StateMachineCompact` library is used, and the source can be found in this repo. 
It is a small library with powerful Macros to easily define the State Machine. Please see `Pod_State.cpp` for our implementation.

# Code Walkthrough 
`Pod.cpp` contains the `main()` function. This calls the `Pod::startup()` method which startsup all other threads:
  * `logic_loop()` Process incoming commands or errors, run steady-state function, calculate pod position/ velocity.
  * `tcp_send` and `tcp_recv` Two threads for async TCP operation. 
  * `udp_connection` one UDP thread for maintaining a heartbeat with the basestation
  * One thread per `SourceManager`

A `SourceManager` is a sensor, such as ADC, I2C, CANBus, or PRU. `SourceManagerBase.hpp` is a parent class for all of the SourceManagers, since they all share most of the same functionality: `initialize_source()` open device; `refresh()` get new data; `stop_source()` close device. These are all threads, with a configurable sleep time between each `refresh()`.

`SourceManager.cpp` and `SourceManager.hpp` are used to create global singleton classes of each SourceManager, such that pointers or references did not need to be passed around. 

`Configurator.cpp` is a simple dictionary that stores configurable values, which will load by default from `defaultConfig.txt`.

`Brakes.cpp` and `Motor.cpp` serve to be wrappers for the underlying brake/ motor interface. 

# Overview of testing structure
Google Test is used to facilitate testing. Testing is enabled by a compile-time flag, which will run the tests defined in the `tests/` folder. 

`PodTest.cpp` defines the setup and teardown code for many of the tests. 

`Simulator.cpp` defines a basestation that send commands to the Pod via TCP, and has several hooks to appropriatly test motor/ brake control. 

The test environment can easily access all of the global pod structures to verify proper operation.

`Events.cpp` are used as a sychronization primative to prevent race conditions. For example, when the Simulator sends a TCP command, the event must fire before the test checks a condition. 

# StyleGuide
Using [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html). 
Specifically we use `cpplint.py` for easy linting of the most obvious errors. We include Google's repository containting `cpplint.py` as a git submodule.
* To run the linter, run `make lint`



# Building the project using the Makefile
* Simply run `make` to get started. This will first compile the GTest library, and then the debug and test versions of the codebase.

### There are two types of executables
* The `build` uses whatever default version of `g++` is installed on your system
* The `cross` uses the `arm-linux-gnueabihf-g++` compiler and associated tool chain, so it can be compiled on a host machine and copied over to the 32-bit ARM Beaglebone Black

### There are several compile time flag options
* Release: 
  * defines `NDEBUG`, and compiles with optimizations
  * `build` and `cross`
* Debug:
  * defines `DEBUG`, and compiles without optimizations
  * `dbuild` and `dcross`
* Simulation:
  * defines `DEBUG` and `SIM`, and compiles without optimizations
  * `sbuild` and `scross`
* BeagleBoneBlack:
  * defines `BBB` when cross compiling, along with one of the above modes
  * Note: No Sanitizers are used while compiling for the BBB. They are not implemented for 32-bit ARM.

### All make options
* `make`, will compile the debug and test versions of the code, with the ThreadSanitizer already enabled.
* `make build`, `make dbuild`, `make sbuild`, will compile the release, debug, or test versions, respectively. 
  * Can run `make [exe_name]-tsan` or `make [exe_name]-asan` to build with those sanitizers. Be sure to run `make clean` when switching between sanitizer options.
* `make cross`, `make dcross`, `make scross`, will cross compile the release, debug, or test versions, respectively. 
  * There are no Sanitizers for 32-bit ARM. 

### Other commands:
* `make clean` cleans up all executables and objects
* `make clean_lib` cleans up the GTest library
* `make push` uses scp to copy all `cross` executables over to a Beaglebone

