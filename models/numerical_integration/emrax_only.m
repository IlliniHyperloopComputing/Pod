
%Change parameters below
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
runlength = 1200; %Assume 1.25 km track, want to leave 50 meter buffer at the end

%%%%%%%%%
%Emrax Specific
%%%%%%%%%
EmraxGear = 2.79;%2.6:0.01:2.9;%[2.79];
EmraxRadius = 0.2032/2; %wheel Radius (meters)
EmraxMaxMechPower    = [50]; %PEAK MECHANICAL power, in kW. Mechanical power WILL NOT go over this value
EmraxMaxBatteryPower = [96]; % Max BATTERY OUTPUT in kw. THIS THROTTLES THE EMRAX. The BATTERY OUTPUT WILL NOT GO OVER THIS VALUE
EmraxPeakTorque = 230; %Peak torque, in Nm. 
EmraxMaxRPM = 4400; %Max possible RPM under load. Past this RPM we assume 0 output power
EmraxKV = 34;
EmraxBatteryVoltage = 120; %Starting voltage
EmraxBatteryAH = 22; 
EmraxBatteryResistance = [0.05]; % P = I^2 * R
EmraxControllerEfficiency = 0.95;


%%%%%%%%%
%Pod Mass (kg)
mass = 135; 


%%%%%%%%%
%Pod Braking parameters
BrakingForce = 1265; % In N
MaxBrakingGs = BrakingForce/mass/9.80665;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%
%Numerical integration delta time
dt = 0.1; %delta time
%%%%%%

% Key variables to keep track of
Best_AccelerationTime = 0;
Best_GearEmrax = 0;
Best_TopSpeed = 0;
Best_AccelerationDistance = 0;
Best_ResistanceBattery = 0;
Best_MaxMechPower = 0;
Best_MaxBatteryPower = 0;
Best_HeatEmrax = 0;
Best_HeatEmsiso = 0;
Best_HeatBattery = 0;

%%%%% Plot things at the end
Plot_things = true;
%%%%%

fprintf('Time | GEAR_EMRAX | TopSpeed(m/s) | AccelDistance(m) | BrakeDistance(m) | MaxMechPower(W) | MaxBatteryPower(W) | HeatEmrax(J) | HeatEmissio(J) | HeatBattery(J) \n');

for emraxBatteryResistance = EmraxBatteryResistance
for p_opt = 1:length(EmraxMaxMechPower)
for emraxGear = EmraxGear
    
    emraxMaxMechPower    = EmraxMaxMechPower(p_opt);
    emraxMaxBatteryPower = EmraxMaxBatteryPower(p_opt);

    t = 0;
    d = 0;
    v = 0;
    a = 0;

    time = [];
    distance = [];
    velocity = [];
    acceleration = [];
    
    %%%%% Emrax power variables
    vec_emraxMechPower = [];
    vec_emraxMotorInputPower = [];
    vec_emraxControllerInputPower = [];
    vec_emraxCurrent = [];
    vec_emraxVoltage = [];

    emraxBatteryVoltage = EmraxBatteryVoltage;
    
    heatEmrax = 0;
    heatEmsiso = 0;
    heatEmraxBattery = 0;
    
    while d < runlength %track length in meters
        
        %%%%%%%%%%%%%%%%%%%%%%%%
        %%%%%%%% Emrax
        %%%%%%%%%%%%%%%%%%%%%%%%
        emraxWheelRPM = v * 60 /(2*pi*EmraxRadius); %calculate wheel RPM at point in time
        emraxRPM = emraxWheelRPM / emraxGear;
        
        battery_voltage_including_sag = emraxBatteryVoltage - emraxCurrent *emraxBatteryResistance;
        maxRPM_at_voltage = EmraxKV * battery_voltage_including_sag;
        
        % Apply motor dynamics. This will throttle the power output
        [emraxTorque, emraxMechPower, emraxMotorInputPower, emraxControllerInputPower, emraxCurrent] = ...
          Motor_Dynamics(emraxRPM, emraxMaxMechPower, EmraxPeakTorque, maxRPM_at_voltage, EmraxControllerEfficiency...
                          ,emraxBatteryVoltage, emraxBatteryResistance, emraxMaxBatteryPower, true);
      
        heatEmrax        = heatEmrax        + (emraxMotorInputPower - emraxMechPower) * dt;
        heatEmsiso       = heatEmsiso       + (emraxControllerInputPower - emraxMotorInputPower) * dt;
        heatEmraxBattery = heatEmraxBattery + ((emraxCurrent*emraxBatteryVoltage) - emraxControllerInputPower) * dt;
      
        % Store all of the Emrax variables
        if(Plot_things)
          vec_emraxMechPower             = [vec_emraxMechPower            emraxMechPower];
          vec_emraxMotorInputPower       = [vec_emraxMotorInputPower      emraxMotorInputPower];
          vec_emraxControllerInputPower  = [vec_emraxControllerInputPower emraxControllerInputPower];
          vec_emraxCurrent               = [vec_emraxCurrent                   emraxCurrent];
          vec_emraxVoltage               = [vec_emraxVoltage                   emraxBatteryVoltage];
        end
        
        % Update battery voltage
        emraxBatteryVoltage = emraxBatteryVoltage  - 1.0/(EmraxBatteryAH*3600) * emraxCurrent*dt;
        
        % Determine torque at the wheel
        emraxTorqueWheel = emraxTorque / emraxGear;
        
        
        %%%%%%%%%%%%%%%%%%%%%%%%
        % Increment Time
        time = [time t];
        t = t + dt;
       
        % Caluclate traction force
        emraxTractionForce = emraxTorqueWheel / EmraxRadius;

        % Apply Kinematics
        acceleration = [acceleration a];
        a = emraxTractionForce/mass; 
        
        velocity = [velocity v];
        v = v + a * dt;
        
        distance = [distance d];
        d = d + v * dt + .5 * a * dt^2;
    end
    
    % Determines moment when braking needs to start
    gtotal = (velocity.^2) ./ (2 .* 9.80665 .* (runlength - distance));
    [g,i] = min(abs(gtotal - MaxBrakingGs)); 
    g = g + MaxBrakingGs;

    % Store the Maximum velocity
    topSpeed = velocity(i);
    if(topSpeed > Best_TopSpeed)
        Best_AccelerationTime = time(i);
        Best_GearEmrax = emraxGear;
        Best_TopSpeed = topSpeed;
        Best_AccelerationDistance = distance(i);
        Best_ResistanceBattery = emraxBatteryResistance;
        Best_MaxMechPower = emraxMaxMechPower;
        Best_MaxBatteryPower = emraxMaxBatteryPower;
        Best_HeatEmrax = heatEmrax;
        Best_HeatEmsiso = heatEmsiso;
        Best_HeatBattery = heatEmraxBattery;
    end

    % Propogate braking further
    stoptime = sqrt(2*(runlength-distance(i))/9.80665/g);
    time = 0:dt:(time(i)+stoptime);
    
    for j = i:length(time)
      acceleration(j) = -BrakingForce/mass;
      velocity(j) = velocity(j-1) + acceleration(j) * dt;
      distance(j) = distance(j-1) + velocity(j) * dt + .5 * acceleration(j) * dt^2;
    end
    

    %%%%%%%%Comment out the following to remove plots
    if(Plot_things)
        % Begin Plots
        
        % Kinematics Plots
        %figure(1, 'position', [0,0, 1600, 1200]);
        figure ;
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
        
        % Emrax Specific Plots
        % figure(2, 'position', [0,0, 1600, 1200]);
        figure ;
        s1 = subplot (2, 2, 1);
        emrax_rpm = velocity * 60 /(2*pi*EmraxRadius) / emraxGear;
        emrax_torque = vec_emraxMechPower(1:i)./(emrax_rpm(1:i)/60 * 2 * pi);
        plot(emrax_rpm(1:i), emrax_torque(1:i));
        str = strcat("Emrax_rpm vs. emrax torque at gear: ", mat2str(emraxGear));
        title(str);
        xlabel("RPM");
        ylabel("Torque");
        Plot_Torque_Curve();

        s2 = subplot (2, 2, 3);
        str = strcat("Emrax power at gear: ", mat2str(emraxGear));
        title(str);
        xlabel("time");
        ylabel("Power");
        hold(s2,'on');
        plot(time(1:i), vec_emraxMechPower(1:i), 'color', 'g');
        plot(time(1:i), vec_emraxMotorInputPower(1:i), 'color', 'm');
        plot(time(1:i), vec_emraxControllerInputPower(1:i), 'color', 'k');
        plot(time(1:i), vec_emraxVoltage(1:i) .* vec_emraxCurrent(1:i), 'color', 'r');
        legend("MechPowerEmrax", "MotorInputPowerEmrax", "ControllerInputPowerEmrax", "Battery Power", 'location', 'southeast');

        s3 = subplot (2, 2, 2);
        str = strcat("Emrax voltage vs. time at gear: ", mat2str(emraxGear));
        title(str);
        xlabel("time");
        ylabel("voltage");
        hold(s3,'on');
        plot(time(1:i), vec_emraxVoltage(1:i) , 'color', 'g');
        plot(time(1:i), vec_emraxVoltage(1:i) - vec_emraxCurrent(1:i)*emraxBatteryResistance , 'color', 'r');
        legend("PackVoltage", "Voltage with sag", 'location', 'southeast');

        s4 = subplot (2, 2, 4);
        str = strcat("Emrax currant vs. time at gear: ", mat2str(emraxGear));
        title(str);
        xlabel("time");
        ylabel("amps");
        hold(s4,'on');
        plot(time(1:i), vec_emraxCurrent(1:i) , 'color', 'r');


    end
    fprintf('%.2f %6.2f %15.2f %17.4f %18.2f %17.2f %20.2f %18.2f %15.2f %14.2f  ',...
            time(i), emraxGear, topSpeed,distance(i), runlength-distance(i), ...
            emraxMaxMechPower, emraxMaxBatteryPower,...
            heatEmrax, heatEmsiso, heatEmraxBattery);
    disp(" ");
end
end
end

fprintf('Time | GEAR_EMRAX | TopSpeed(m/s) | AccelDistance(m) | BrakeDistance(m) | MaxMechPower(W) | MaxBatteryPower(W) | HeatEmrax(J) | HeatEmissio(J) | HeatBattery(J) \n');

fprintf('%.2f %6.2f %15.2f %17.4f %18.2f %17.2f %20.2f %18.2f %15.2f %14.2f  ',...
        Best_AccelerationTime, Best_GearEmrax, Best_TopSpeed, Best_AccelerationDistance, Best_ResistanceBattery, Best_MaxMechPower,...
        Best_MaxBatteryPower, Best_HeatEmrax, Best_HeatEmsiso, Best_HeatBattery);
disp(" ");
