function [efficiency] = Emrax_Efficiency_Map(torque, rpm)
    % Where x==RPM   y==Torque
global poly_96_x;
global poly_96_y;
global poly_95_x;
global poly_95_y;
global poly_94_x;
global poly_94_y;

global poly_90_x;
global poly_90_y;

global poly_86_x;
global poly_86_y;

    poly_96_x = [1500 1750 3200 3550 2100 1900 1500];
    poly_96_y = [125 130 120 110 80 85 125];
    poly_95_x = [1900 1400 1200 2500 4000 4250 4000 1900];
    poly_95_y = [60 100 150 165 152 125 100 60];
    poly_94_x = [4000 3500 1500 1000 1000 1100 2000 5000 5000 4000];
    poly_94_y = [70 60 60 70 140 165 200 195 150 70];

    poly_90_x = [5000 5000 3500 1500 1000 600 1400 5000];
    poly_90_y = [230 120 45 45 55 110 235 230];

    poly_86_x = [5000 5000 3600 1000 300 400 1200 5000];
    poly_86_y = [245 105 26 25 50 150 240 245];


    in_96 = inpolygon(rpm, torque, poly_96_x, poly_96_y);
    in_95 = inpolygon(rpm, torque, poly_95_x, poly_95_y);
    in_94 = inpolygon(rpm, torque, poly_94_x, poly_94_y);
    in_90 = inpolygon(rpm, torque, poly_90_x, poly_90_y);
    in_86 = inpolygon(rpm, torque, poly_86_x, poly_86_y);
    
    if(in_96)
        efficiency = 0.96;
    elseif(in_95)
        efficiency = 0.95;
    elseif(in_94)
        efficiency = 0.94;
    elseif(in_90)
        efficiency = 0.90;
    elseif(in_86)
        efficiency = 0.86;
    else
        efficiency = 0.80;
    end
end
