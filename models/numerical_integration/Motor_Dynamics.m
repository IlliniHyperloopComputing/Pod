
function [Torque, MechPower, MotorInputPower, ControllerInputPower, Current] ...
          = Motor_Dynamics (RPM, MechPowerTarget, PeakTorque, MaxRPM, ControllerEfficiency...
                            ,BatteryVoltage, BatteryResistance, BatteryMaxPower, isEmrax)
  
  MechPowerTarget = MechPowerTarget+0.1; %measured in kw, this is adding 100w
  BatteryPower = BatteryMaxPower*1000 *2; %this gets overwritten, justs needs to be initally higher than BatteryMaxPower
  
  %This function will determine the maximum torque that can be applied,
  %such that the battery output is no higher than the given maximum. 
  %Essentially, this is the "lazy" way of doing this by just repeatedly
  %trying a bunch of values.
  %
  %This alg is _Really_ slow if MechPowerTarget == BatteryMaxPower, since
  %it will have to iterate downward. 
  %
  %I didn't spend any time trying to optimize
  
  while(BatteryPower > BatteryMaxPower*1000)
    MechPowerTarget = MechPowerTarget - 0.1;
  
    [Torque, MechPower] = Torque_curve_reader(RPM, MechPowerTarget, PeakTorque, MaxRPM); %rpm, Motor Power (kW), Max Torque (Nm), Max RPM, gear ratio
    if(isEmrax)
      Efficiency = Emrax_Efficiency_Map(Torque, RPM);
    else
        %BIG assumption here, but I believe it is a safe one
      Efficiency = 0.8;
    end
    
    MotorInputPower = MechPower/Efficiency;
    ControllerInputPower = MotorInputPower/ControllerEfficiency;
    
    % Solve for Current using from this formula:
    % P = V_pack*I_out - I_out^2*R_pack 
    Current = (BatteryVoltage - sqrt(BatteryVoltage^2 - 4*BatteryResistance * ControllerInputPower))/(2*BatteryResistance);
    
    BatteryPower = Current * BatteryVoltage;
  end

end
