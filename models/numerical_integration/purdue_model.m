TopSpeedVec = [];
AccelerationDistanceVec = [];
BrakingForceVec = [];
gVec = [];
AccelerationRunTime = [];
EmraxGearIdx = [];
TP100GearIdx = [];

%Change parameters below
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
gearVecTp100 = 0.7;%0.68:0.01:0.72;%0.8%0.4:0.1:0.9; %loop through gear ratios %(0.7:130m/s for just 6 tp100s)
gearVecEmrax = 4.1;%3.8:0.01:4.2;%4.5%3.8:0.1:4.5; %(3.3:115m/s for just emrax)
TP100GearRatio = 1; %Assuming direct drive
runlength = 1200; %Assume 1.25 km track, want to leave 50 meter safety at the end

radiusEmrax = 0.0762; %wheel Radius (meters)
radiusTP100 = 0.0762; %wheel Radius (meters)



%%%%%%%%%
%Emrax Specific
%%%%%%%%%
EmraxPower = 50; %PEAK MECHANICAL power, in kW. Mechanical power WILL NOT go over this value
EmraxBatteryMaxPower = 100; % Max BATTERY OUTPUT in kw. THIS THROTTLES THE EMRAX. The BATTERY OUTPUT WILL NOT GO OVER THIS VALUE
EmraxPeakTorque = 240; %Peak torque, in Nm
EmraxMaxRPM = 4400; %Max RPM under load
EmraxWeight = 12; %kg
EmsisoWeight = 4.9;
EmraxBatteryWeight = 20; %weight in kg
EmraxBatteryVoltage = 125; %Starting voltage
EmraxBatteryAH = 5.5*4; 
EmraxBatteryResistance = 0.05; % P = I^2 * R
EmraxControllerEfficiency = 0.95;

%Iterate through these didfferent power options
emrax_mech_power        = [EmraxPower];%[ 45  50  55  60  65  70 ];
emrax_battery_max_power = [EmraxBatteryMaxPower];%[ 100 100 100 100 100 100 ];

%Iterate through these different resistance values
emraxResistanceVec = [0.04];%%[0.00001 0.01 0.02 0.03 0.04 0.05];

%%%%%%%%%
%TP100 Specific
%%%%%%%%%
TP100Power = 20; %PEAK MECHANICAL power, in kW. Mechanical power WILL NOT GO OVER THIS VALUE
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

%%%%%%%%%
%Pod Mass (kg), Right now its (frame and structure) + Emrax + Emsiso + Emrax battery + TP100 system
mass = 60 + EmraxWeight + EmsisoWeight + EmraxBatteryWeight + TP100AvgWeight*TP100Num + TP100AvgBatteryWeight*TP100Num;
%mass = 60 + EmraxWeight + EmsisoWeight + EmraxBatteryWeight; 
%mass = 60 + TP100AvgWeight*TP100Num + TP100AvgBatteryWeight*TP100Num; 
maxG = 5;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%
dt = 0.1; %delta time
%%%%%%
disp("ALL VALUES AND GRAPHS OF TP100 ARE FOR A SINGLE TP100");
fprintf('Time | GEAR_EMRAX | GEAR_TP100 | TopSpeed(m/s) | BrakingForce(N) | AccelDistance(m) | BrakeDistance(m) | MaxGForce | heatEmrax | heatEmissio | heatEmraxBattery | heatTP100 | heatTP100ESC | heatTP100Battery \n');

for em_r = emraxResistanceVec

EmraxBatteryResistance = em_r;
fprintf("EMRAX BATTERY RESISTANCE: %.3f", EmraxBatteryResistance);
disp(" \n");

for p_opts = 1:length(emrax_mech_power)

  EmraxPower = emrax_mech_power(p_opts);
  EmraxBatteryMaxPower = emrax_battery_max_power(p_opts);
  fprintf("POWER OPTION %.2f EmraxMech: %.2f  EmraxBatteryMaxPower: %.2f", p_opts, EmraxPower, EmraxBatteryMaxPower);
  disp("\n");

for gear_emrax = gearVecEmrax
for gear_tp100 = gearVecTp100

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
%        if(t <=1.0+dt && t>=1.0)
%          fprintf("#");
%        end
%        if(t <=5.0+dt && t>=5.0)
%          fprintf(" ##");
%        end
%        if(t <=7.0+dt && t>=7.0)
%          fprintf(" ###");
%        end
%        if(t <=10.0+dt && t>=10.0)
%          fprintf(" ####");
%        end
%         if(t <=12.0+dt && t>=12.0)
%          disp(" #####");
%        end
        
        
        %%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%% Emrax
        %%%%%%%%%%%%%%%%%%%%%%%%
        EmraxWheelRPM = v * 60 /(2*pi*radiusEmrax); %calculate wheel RPM at point in time
        EmraxRPM = EmraxWheelRPM / gear_emrax;
        
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
        TorqueEmraxWheel = TorqueEmrax / gear_emrax;
        
        
        %%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%% TP100
        %%%%%%%%%%%%%%%%%%%%%%%%
        TP100WheelRPM = v * 60 /(2*pi*radiusTP100); %calculate wheel RPM at point in time
        TP100RPM = TP100WheelRPM / gear_tp100;        
        
        %TP100RPM = v * 60 / (2*pi*radiusTP100);
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
        TorqueTP100Wheel = TorqueTP100 / gear_tp100;
        TractionForceTP100 = TorqueTP100Wheel / radiusTP100;

        % Apply Kinematics
        acceleration = [acceleration a];
        a = TractionForceEmrax/mass + TP100Num * TractionForceTP100/mass;
        
        
        velocity = [velocity v];
        v = v + a * dt;
        
        distance = [distance d];
        d = d + v * dt + .5 * a * dt^2;
    end
    
    gtotal = velocity.^2./(2 .* 9.80665 .* (runlength .- distance));
    [g,i] = min(abs(gtotal - maxG));
    g = g + maxG;
    TopSpeed = velocity(i);
    
    AccelerationDistance = distance(i);
    
    TopSpeedVec = [TopSpeedVec TopSpeed];
    
    AccelerationRunTime = [AccelerationRunTime time(i)];
    
    stoptime = sqrt(2*(runlength-distance(i))/9.81/g);
    
    time = 0:dt:(time(i)+stoptime);
    
    BrakingForce = (0.5 * mass * TopSpeed^2) ./ (runlength - AccelerationDistance);
    
    BrakingForceVec = [BrakingForceVec BrakingForce];
    
    AccelerationDistanceVec = [AccelerationDistanceVec AccelerationDistance];
    gVec = [gVec g];
    
    EmraxGearIdx = [EmraxGearIdx gear_emrax];
    TP100GearIdx = [TP100GearIdx gear_tp100];
    
    for j = i:length(time)
      acceleration(j) = -BrakingForce/mass;
      velocity(j) = velocity(j-1) + acceleration(j) * dt;
      distance(j) = distance(j-1) + velocity(j) * dt + .5 * acceleration(j) * dt^2;
    end
    

    %%%%%%%%Comment out the following to remove plots
    Plot_things = true;
    if(Plot_things)
        % Begin Plots
        
        figure(2, 'position', [0,0, 1600, 1200]);
        %Emrax:
        s1 = subplot (2, 2, 1);
        plot(time, distance);
        str = strcat("distance vs time");
        title(str);
        xlabel("time (s)");
        ylabel("distance (m)");

        s2 = subplot (2, 2, 2);
        str = strcat("velocity vs time");
        title(str);
        xlabel("time (s)");
        ylabel("velocity (m/s)");
        hold(s2,'on');
        plot(time, velocity, 'color', 'g');
        
        s3 = subplot (2, 2, 3);
        str = strcat("acceleration vs time");
        title(str);
        xlabel("time (s)");
        ylabel("acceleration (m/s^2)");
        hold(s3,'on');
        plot(time, acceleration, 'color', 'r');
        
        figure(1, 'position', [0,0, 1600, 1200]);
        %Emrax:
        s1 = subplot (2, 4, 1);
        emrax_rpm = velocity * 60 /(2*pi*radiusEmrax) / gear_emrax;
        emrax_torque = mechPowerEmrax(1:i)./(emrax_rpm(1:i)/60 * 2 * pi);
        plot(emrax_rpm(1:i), emrax_torque(1:i));
        str = strcat("Emrax_rpm vs. emrax torque at gear: ", mat2str(gear_emrax));
        title(str);
        xlabel("RPM");
        ylabel("Torque");
        Plot_Torque_Curve();

        s2 = subplot (2, 4, 5);
        str = strcat("Emrax power at gear: ", mat2str(gear_emrax));
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
        str = strcat("Emrax voltage vs. time at gear: ", mat2str(gear_emrax));
        title(str);
        xlabel("time");
        ylabel("voltage");
        hold(s3,'on');
        plot(time(1:i), emraxVoltage(1:i) , 'color', 'g');
        plot(time(1:i), emraxVoltage(1:i) - emraxCurrent(1:i)*EmraxBatteryResistance , 'color', 'r');
        legend("PackVoltage", "Voltage with sag", 'location', 'southeast');

        s4 = subplot (2, 4, 6);
        str = strcat("Emrax currant vs. time at gear: ", mat2str(gear_emrax));
        title(str);
        xlabel("time");
        ylabel("amps");
        hold(s4,'on');
        plot(time(1:i), emraxCurrent(1:i) , 'color', 'r');


        %TP100s
        s5 = subplot (2, 4, 3);
        tp100_rpm = velocity * 60 /(2*pi*radiusTP100);
        tp100_torque = mechPowerTP100(1:i)./(tp100_rpm(1:i)/60 * 2 * pi);
        plot(tp100_rpm(1:i), tp100_torque(1:i));
        str = strcat("tp100_rpm vs. tp100 torque at gear: ", mat2str(gear_tp100));
        title(str);
        xlabel("RPM");
        ylabel("Torque");

        s6 = subplot (2, 4, 7);
        str = strcat("tp100 power at gear: ", mat2str(gear_tp100));
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
        str = strcat("tp100 voltage vs. time at gear: ", mat2str(gear_tp100));
        title(str);
        xlabel("time");
        ylabel("voltage");
        hold(s7,'on');
        plot(time(1:i), tp100Voltage(1:i) , 'color', 'g');
        plot(time(1:i), tp100Voltage(1:i) - tp100Current(1:i)*TP100AvgBatteryResistance , 'color', 'r');
        legend("PackVoltage", "Voltage with sag", 'location', 'southeast');

        s8 = subplot (2, 4, 8);
        str = strcat("TP100 currant vs. time at gear: ", mat2str(gear_tp100));
        title(str);
        xlabel("time");
        ylabel("amps");
        hold(s8, 'on');
        plot(time(1:i), tp100Current(1:i) , 'color', 'r');
    end
    %%%%%COmment out the above to remove plotting    

    fprintf('%.2f %10.2f %10.2f %17.4f %18.2f %17.2f %18.2f %13.2f %13.2f %13.2f %10.2f %18.2f %13.2f %10.2f %18.2f',...
            time(i),gear_emrax, gear_tp100, TopSpeed,BrakingForce,AccelerationDistance, runlength-AccelerationDistance,g,...
            heatEmrax, heatEmissio, heatEmraxBattery,...
            heatTP100, heatTP100Controller, heatTP100Battery);
    disp(" ");
end
end
end
end

[maxTopSpeed, k] = max(TopSpeedVec);

fprintf('Time | Emrax__Ratio | TP100__Ratio | TopSpeed(m/s) | BrakingForce(N) | AccelDistance(m) | BrakeDistance(m) | MaxGForce \n')
fprintf('Max:%.2f %7.2f %10.2f %17.2f %18.2f %17.2f %18.2f %13.2f\n', AccelerationRunTime(k), EmraxGearIdx(k), TP100GearIdx(k), TopSpeedVec(k),BrakingForceVec(k),AccelerationDistanceVec(k), runlength-AccelerationDistanceVec(k),gVec(k))