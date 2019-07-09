# Numerical Integration scripts for modeling Pod Dynamics

The most important file is `emrax_only.m`. Not only is this the most up to date model (when compared to `emrax_tp100.m` and `purdue_model.m`), but it is also the most accurate. 
-  `Emrax_Efficiency_Map.m` takes the established efficiency regions of the EMRAX 228 motor from the datasheet, and graphs it. This allows for the calling script to input an RPM and Torque, and get the efficiency percent back
-  `Motor_Dynamics.m` Helps to determines the maximum torque that can be applied, based on the input parameters, like maximum operating power and Amperage. This is slow because it wasn't optimized.
-  `Plot_Torque_curve.m` Just plots the EMRAX efficiency regions. It's a helper function.
-  `Torque_curve_reader.m` was a predecesor to `Motor_Dynamics.m` and isn't used right now

This script is setup such that parameters can be optimized. Originally, the gear ratio was the parameter most in question (which is what is iterated over in the `purdue_model.m`). In my effort to make this script more functional, other parameters can be iterated over as well ( like battery resistance, and overall power output). However, I must warn that this script can go quite slow, and there isn't a progress bar. It is especially slow when the MaxMechanicalPower == MaxBatteryPower, because of how poorly the `Motor_Dynamics.m` is optimized (it isn't optimized). 


I will appologize ahead of time if it is confusing how the script even works. I (Richard Wendel) modified the original `purdue_model.m` to fit what I wanted. I added much more details about power and efficiency.

Hopefully this is usefull to someone in the future, even if the lead time to being able to fully use it is a bit long.




