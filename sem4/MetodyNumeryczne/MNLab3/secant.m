function [xvect,xdif,fx_current,it_cnt]=secant(a,b,eps,fun)

xvect = [];
xdif = [];
fx = [];
it_cnt = 0;
x = b;
x_prev = a;
for i = 1:1000
    % secant algorithm 
    
    fx_current = feval(fun,x);
    fx_prev = feval(fun,x_prev);

    it_cnt = it_cnt + 1;
    fx(end+1) = fx_current;
    xvect(end+1) = x;
    xdif(end+1) = abs(x_prev - x);

    if abs(fx_current) < eps
        break;
    end
    
    x_new = x -((fx_current * (x - x_prev))/(fx_current - fx_prev));
    x_prev = x;
    x = x_new;
end
end