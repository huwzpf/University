function [xvect,xdif,fx_current,it_cnt]=bisect(a,b,eps,fun)

xvect = [];
xdif = [];
fx = [];
it_cnt = 0;
l = a;
r = b;
x = (a + b)/2;
x_prev = x;
for i = 1:1000
    % bisection algorithm 
    
    fx_current = feval(fun,x);
    fl = feval(fun,l);
    if abs(fx_current) < eps
        break;
    end
    
    if fl * fx_current < 0
        r = x;
    else
        l = x;
    end

    x = (l + r)/2;
    it_cnt = it_cnt + 1;
    fx(end+1) = fx_current;
    xvect(end+1) = x;
    xdif(end+1) = abs(x_prev - x);
    x_prev = x;
end
end

