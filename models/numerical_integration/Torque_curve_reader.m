function [torque, power] = Torque_curve_reader(Given_RPM, Power_Kw, Torque_max, RPM_max)
    Power = Power_Kw*1000;
    critical_w = (Power)/Torque_max;
    
    w = (Given_RPM.*2.*pi)./60;
    w_max = (RPM_max.*2.*pi)./60;

    if w<critical_w
        torque = Torque_max;
    elseif w > w_max
        torque = 0;
    else
        torque = Power./w;
    end
    
    power = w .* torque;
end