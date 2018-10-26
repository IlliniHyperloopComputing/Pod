TopSpeedVec = [];
AccelerationDistanceVec = [];
BrakingForceVec = [];
gVec = [];
AccelerationRunTime = [];

%Change parameters below
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
gearVec = 3.6:0.1:4.2; %loop through gear ratios for the Emrax
TP100GearRatio = 1; %Assuming direct drive
runlength = 1200; %Assume 1.25 km track, want to leave 50 meter safety at the end

radiusEmrax = 0.0762; %wheel Radius (meters)
radiusTP100 = 0.0762; %wheel Radius (meters)

powerVariables = 1:1:5;

EmraxPower = 60; %PEAK MECHANICAL power, in kW. Mechanical power WILL NOT go over this value
EmraxBatteryMaxPower = 70; % Max BATTERY OUTPUT in kw. THIS THROTTLES THE EMRAX. The BATTERY OUTPUT WILL NOT GO OVER THIS VALUE
EmraxPeakTorque = 240; %Peak torque, in Nm
EmraxMaxRPM = 4400; %Max RPM under load
EmraxWeight = 12; %kg
EmsisoWeight = 4.9;
EmraxBatteryWeight = 20; %weight in kg
EmraxBatteryVoltage = 125; %Starting voltage
EmraxBatteryAH = 5.5*4; 
EmraxBatteryResistance = 0.05; 
EmraxControllerEfficiency = 0.95;

TP100Power = 25; %PEAK MECHANICAL power, in kW. Mechanical power WILL NOT GO OVER THIS VALUE
TP100AvgBatteryMaxPower = 25; % % Max BATTERY. THIS THROTTLES A TP100. in kW. BATTERY OUTPUT WILL NOT GO OVER THIS VALUE
TP100PeakTorque = 9.75; %Peak torque, in Nm
TP100MaxRPM = 25000; %Max RPM under load
TP100Num = 6; %NUMBER OF TP100s
TP100AvgWeight = (28)/6.0; % According to UC's powerpoint
TP100AvgBatteryWeight = 30/6.0;
TP100BatteryVoltage = 85;
TP100AvgBatteryAH = 5.5*3/2.0;
TP100AvgBatteryResistance = 0.03;%
TP100ControllerEfficiency = 0.95;

%Pod Mass (kg), Right now its (frame and structure) + Emrax + Emsiso + Emrax battery + TP100 system
mass = 60 + EmraxWeight + EmsisoWeight + EmraxBatteryWeight + TP100AvgWeight*TP100Num + TP100AvgBatteryWeight*TP100Num; 
maxG = 5;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%
dt = 0.1; %delta time
%%%%%%
disp("ALL VALUES AND GRAPHS OF TP100 ARE FOR A SINGLE TP100");
fprintf('Time | TransmissionRatio | TopSpeed(m/s) | BrakingForce(N) | AccelDistance(m) | BrakeDistance(m) | MaxGForce | heatEmrax | heatEmissio | heatEmraxBattery | heatTP100 | heatTP100ESC | heatTP100Battery \n');
for p_opts = powerVariables
if(p_opts ==1)
  disp("POWER OPTION 1");
  EmraxPower = 45;
  EmraxBatteryMaxPower = 50;
elseif(p_opts==2)
  disp("POWER OPTION 2");
  EmraxPower = 55;
  EmraxBatteryMaxPower = 60;

elseif(p_opts==3)
  disp("POWER OPTION 3");
  EmraxPower = 60;
  EmraxBatteryMaxPower = 70;

elseif(p_opts==4)
  disp("POWER OPTION 4");
  EmraxPower = 60;
  EmraxBatteryMaxPower = 80;

elseif(p_opts==5)
  disp("POWER OPTION 5");
  EmraxPower = 65;
  EmraxBatteryMaxPower = 90;

end

for gear = gearVec

    t = 0;
    d = 0;
    v = 0;
    a = 0;

    time = [];
    distance = [];
    velocity = [];
    acceleration = [];
    
    %%%%% Emrax power variables
    mechPowerEmrax = [];
    motorInputPowerEmrax = [];
    controllerInputPowerEmrax = [];
    emraxBatteryVoltage = EmraxBatteryVoltage;
    emraxCurrent = [];
    emraxVoltage = [];
    
    heatEmrax = 0;
    heatEmissio = 0;
    heatEmraxBattery = 0;
    
    
    %%%%% TP100 power variables
    mechPowerTP100 = [];
    motorInputPowerTP100 = [];
    controllerInputPowerTP100 =    [];
    tp100BatteryVoltage = TP100BatteryVoltage;
    tp100Current = [];
    tp100Voltage = [];
    
    heatTP100 = 0;
    heatTP100Controller = 0;
    heatTP100Battery = 0;
    
   

    while d < runlength %track length in meters
    
        %To get perspective on how long it has been running
        if(t <=1.0+dt && t>=1.0)
          fprintf("#");
        end
        if(t <=5.0+dt && t>=5.0)
          fprintf(" ##");
        end
        if(t <=7.0+dt && t>=7.0)
          fprintf(" ###");
        end
        if(t <=10.0+dt && t>=10.0)
          fprintf(" ####");
        end
         if(t <=12.0+dt && t>=12.0)
          disp(" #####");
        end
        
        
        %%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%% Emrax
        %%%%%%%%%%%%%%%%%%%%%%%%
        EmraxWheelRPM = v * 60 /(2*pi*radiusEmrax); %calculate wheel RPM at point in time
        EmraxRPM = EmraxWheelRPM / gear;
        
        % Apply motor dynamics. This will throttle the power output
        [TorqueEmrax, MechPowerEmrax, MotorInputPowerEmrax, ControllerInputPowerEmrax, EmraxCurrent] = ...
          Motor_Dynamics(EmraxRPM, EmraxPower, EmraxPeakTorque, EmraxMaxRPM, EmraxControllerEfficiency...
                          ,EmraxBatteryVoltage, EmraxBatteryResistance, EmraxBatteryMaxPower, true);
      
        heatEmrax = heatEmrax+ (MotorInputPowerEmrax - MechPowerEmrax)*dt;
        heatEmissio = heatEmissio+ (ControllerInputPowerEmrax - MotorInputPowerEmrax)*dt;
        heatEmraxBattery = heatEmraxBattery+ ((EmraxCurrent*emraxBatteryVoltage) - ControllerInputPowerEmrax) * dt;
      
        % Store all of the Emrax variables
        mechPowerEmrax = [mechPowerEmrax MechPowerEmrax];
        motorInputPowerEmrax = [motorInputPowerEmrax MotorInputPowerEmrax];
        controllerInputPowerEmrax = [controllerInputPowerEmrax ControllerInputPowerEmrax];
        emraxCurrent = [emraxCurrent EmraxCurrent];
        emraxVoltage = [emraxVoltage emraxBatteryVoltage];
        
        % Update battery voltage
        emraxBatteryVoltage = emraxBatteryVoltage  - 1.0/(EmraxBatteryAH*3600) * EmraxCurrent*dt;
        
        % Determine torque at the wheel
        TorqueEmraxWheel = TorqueEmrax / gear;
        
        
        %%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%% TP100
        %%%%%%%%%%%%%%%%%%%%%%%%
        TP100RPM = v * 60 / (2*pi*radiusTP100);
        [TorqueTP100, MechPowerTP100, MotorInputPowerTP100, ControllerInputPowerTP100, TP100Current] = ...
          Motor_Dynamics(TP100RPM, TP100Power, TP100PeakTorque, TP100MaxRPM, TP100ControllerEfficiency...
                          ,TP100BatteryVoltage, TP100AvgBatteryResistance, TP100AvgBatteryMaxPower, false);
                      
        heatTP100 = heatTP100+ (MotorInputPowerTP100 - MechPowerTP100)*dt;
        heatTP100Controller = heatTP100Controller+ (ControllerInputPowerTP100 - MotorInputPowerTP100)*dt;
        heatTP100Battery = heatTP100Battery+ ((TP100Current*TP100BatteryVoltage) - ControllerInputPowerTP100) * dt;
                          
        % Store all of the TP100 variables
        mechPowerTP100 = [ mechPowerTP100 MechPowerTP100];
        motorInputPowerTP100 = [motorInputPowerTP100 MotorInputPowerTP100];
        controllerInputPowerTP100 = [controllerInputPowerTP100 ControllerInputPowerTP100];
        tp100Current = [tp100Current TP100Current];
        tp100Voltage = [tp100Voltage tp100BatteryVoltage];
             
        % Update battery voltage
        tp100BatteryVoltage = tp100BatteryVoltage  - 1.0/(TP100AvgBatteryAH*3600) * TP100Current*dt;
        
        
        %%%%%%%%%%%%%%%%%%%%%%%%
        % Increment Time
        time = [time t];
        t = t + dt;
       
        % Caluclate traction force
        TractionForceEmrax = TorqueEmraxWheel / radiusEmrax;
        TractionForceTP100 = TorqueTP100 / radiusTP100;

        % Apply Kinematics
        acceleration = [acceleration a];
        a = TractionForceEmrax/mass + TP100Num * TractionForceTP100/mass;
        
        velocity = [velocity v];
        v = v + a * dt;
        
        distance = [distance d];
        d = d + v * dt + .5 * a * dt^2;
    end
    
    gtotal = velocity.^2./(2 .* 9.80665 .* (runlength - distance));
    [g,i] = min(abs(gtotal - maxG));
    g = g + maxG;
    TopSpeed = velocity(i);
    
    AccelerationDistance = distance(i);
    
    TopSpeedVec = [TopSpeedVec TopSpeed];
    
    AccelerationRunTime = [AccelerationRunTime time(i)];
    

    %%%%%%%%Comment out the following to remove plots
    Plot_things = false;
    if(Plot_things)
        % Begin Plots
        %figure(1, 'position', [0,0, 1600, 1200]);
        %Emrax:
        s1 = subplot (2, 4, 1);
        emrax_rpm = velocity * 60 /(2*pi*radiusEmrax) / gear;
        emrax_torque = mechPowerEmrax./(emrax_rpm/60 * 2 * pi);
        plot(emrax_rpm(1:i), emrax_torque(1:i));
        str = strcat("Emrax_rpm vs. emrax torque at gear: ", mat2str(gear));
        title(str);
        xlabel("RPM");
        ylabel("Torque");
        Plot_Torque_Curve();

        s2 = subplot (2, 4, 5);
        str = strcat("Emrax power at gear: ", mat2str(gear));
        title(str);
        xlabel("time");
        ylabel("Power");
        hold(s2,'on');
        plot(time(1:i), mechPowerEmrax(1:i), 'color', 'g');
        plot(time(1:i), motorInputPowerEmrax(1:i), 'color', 'm');
        plot(time(1:i), controllerInputPowerEmrax(1:i), 'color', 'k');
        plot(time(1:i), emraxVoltage(1:i) .* emraxCurrent(1:i), 'color', 'r');
        legend("MechPowerEmrax", "MotorInputPowerEmrax", "ControllerInputPowerEmrax", "Battery Power", 'location', 'southeast');

        s3 = subplot (2, 4, 2);
        str = strcat("Emrax voltage vs. time at gear: ", mat2str(gear));
        title(str);
        xlabel("time");
        ylabel("voltage");
        hold(s3,'on');
        plot(time(1:i), emraxVoltage(1:i) , 'color', 'g');
        plot(time(1:i), emraxVoltage(1:i) - emraxCurrent(1:i)*EmraxBatteryResistance , 'color', 'r');
        legend("PackVoltage", "Voltage with sag", 'location', 'southeast');

        s4 = subplot (2, 4, 6);
        str = strcat("Emrax currant vs. time at gear: ", mat2str(gear));
        title(str);
        xlabel("time");
        ylabel("amps");
        hold(s4,'on');
        plot(time(1:i), emraxCurrent(1:i) , 'color', 'r');


        %TP100s
        s5 = subplot (2, 4, 3);
        tp100_rpm = velocity * 60 /(2*pi*radiusTP100);
        tp100_torque = mechPowerTP100./(tp100_rpm/60 * 2 * pi);
        plot(tp100_rpm(1:i), tp100_torque(1:i));
        str = strcat("tp100_rpm vs. tp100 torque at gear: ", mat2str(gear));
        title(str);
        xlabel("RPM");
        ylabel("Torque");

        s6 = subplot (2, 4, 7);
        str = strcat("tp100 power at gear: ", mat2str(gear));
        title(str);
        xlabel("time");
        ylabel("Power");
        hold(s6,'on');
        plot(time(1:i), mechPowerTP100(1:i), 'color', 'g');
        plot(time(1:i), motorInputPowerTP100(1:i), 'color', 'm');
        plot(time(1:i), controllerInputPowerTP100(1:i), 'color', 'k');
        plot(time(1:i), tp100Voltage(1:i) .* tp100Current(1:i), 'color', 'r');
        legend("MechPowertp100", "MotorInputPowertp100", "ControllerInputPowertp100", "Battery Power", 'location', 'southeast');

        s7 = subplot (2, 4, 4);
        str = strcat("tp100 voltage vs. time at gear: ", mat2str(gear));
        title(str);
        xlabel("time");
        ylabel("voltage");
        hold(s7,'on');
        plot(time(1:i), tp100Voltage(1:i) , 'color', 'g');
        plot(time(1:i), tp100Voltage(1:i) - tp100Current(1:i)*TP100AvgBatteryResistance , 'color', 'r');
        legend("PackVoltage", "Voltage with sag", 'location', 'southeast');

        s8 = subplot (2, 4, 8);
        str = strcat("TP100 currant vs. time at gear: ", mat2str(gear));
        title(str);
        xlabel("time");
        ylabel("amps");
        hold(s8, 'on');
        plot(time(1:i), tp100Current(1:i) , 'color', 'r');
    end
    %%%%%COmment out the above to remove plotting
    
    BrakingForce = (0.5 * mass * TopSpeed^2) ./ (runlength - AccelerationDistance);
    BrakingForceVec = [BrakingForceVec BrakingForce];
    
    AccelerationDistanceVec = [AccelerationDistanceVec AccelerationDistance];
    gVec = [gVec g];
    

    fprintf('%.2f %10.2f %17.4f %18.2f %17.2f %18.2f %13.2f %13.2f %13.2f %10.2f %18.2f %13.2f %10.2f %18.2f',...
            time(i),gear,TopSpeed,BrakingForce,AccelerationDistance, runlength-AccelerationDistance,g,...
            heatEmrax, heatEmissio, heatEmraxBattery,...
            heatTP100, heatTP100Controller, heatTP100Battery);
    disp(" ");
end
end

[maxTopSpeed, k] = max(TopSpeedVec);

fprintf('Time | TransmissionRatio | TopSpeed(m/s) | BrakingForce(N) | AccelDistance(m) | BrakeDistance(m) | MaxGForce \n')
fprintf('Max:%.2f %10.2f %17.2f %18.2f %17.2f %18.2f %13.2f\n', AccelerationRunTime(k), gearVec(k),TopSpeedVec(k),BrakingForceVec(k),AccelerationDistanceVec(k), runlength-AccelerationDistanceVec(k),gVec(k))