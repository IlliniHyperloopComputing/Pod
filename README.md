# Illini Hyperloop Pod Electrical System

## Repo layout:
1. Default branch is `pod4`, and pull requests must be made to merge into it
2. There are tags for older versions of the pod code
3. `master` branch will only be used for correct, working code. A pull request must be made to merge


### This Repo Includes:
1. Flight control software for the BeagleBone Black
  * Network Communication 
  * State machine 
  * Communication to peripheral devices and sensors
2. BeagleBone Black setup
  * General device setup and configuration guide
  * Setup for device tree overlays
  * PRU code
3. Watchdog 
  * Arduino based code for a simple watchdog controller
4. BaseStation
  * GUI basestation for display and control of the Pod
