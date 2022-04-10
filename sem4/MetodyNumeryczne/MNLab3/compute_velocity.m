function [value] = compute_velocity( t )
target_value = 750;
g = 9.81;
m = 150000;
q = 2700;
u = 2000;
value = u * ln(m/(m - (q * t))) - (g * t);
value = value - target_value;
end