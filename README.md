# Illini Hyperloop Pod Electrical System
## How to clone: 
* `git clone https://github.com/IlliniHyperloopComputing/Pod.git FOLDERNAME`

## Repo layout:
1. Default branch is `pod3`, and pull requests must be made to merge into it
2. There are tags for older versions of the pod code
3. `master` branch will only be used for correct, working code. A pull request must be made to merge


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
