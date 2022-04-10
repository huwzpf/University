function [value] = compute_time( N )
target_value = 5000;
value = (N.^(1.43) + N.^(1.14))/1000;
value = value - target_value;
end