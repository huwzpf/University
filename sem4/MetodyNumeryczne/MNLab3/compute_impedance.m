function [value] = compute_impedance( omega )
R = 725;
C = 8e-5;
L = 2;
target_value = 75;
w = sqrt((1/(R.^2)) + (((omega * C) - 1/(omega * L)).^2));
value = 1/w;
value = value - target_value;
end

