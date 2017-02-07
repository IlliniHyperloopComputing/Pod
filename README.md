# Illini Hyperloop Pod Electrical System
## How to clone: 
* `git clone --recursive https://github.com/IlliniHyperloopComputing/Pod.git FOLDERNAME`
* Include `--recursive` to ensure submodule is included

## Repo layout:
1. Default branch is `pod2`
2. `master` branch will only be used for correct, working code. A pull request must be made to merge


### This Repo Includes:
1. Flight control software for the BeagleBone Black
  * Communication between BBB and GUI
  * State machine 
  * Communication to peripheral devices
2. Microcontroller code for XMEGA A1U
  * Active controls
  * Sensor inputs
  * PID control loops
3. LabVIEW GUI
  * Display of all sensor information
  * Full Pod control
4. PCB
  * Design for onboard PCB
  
