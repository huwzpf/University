a = 1;
n_max = 100;
r_max = 0.2;
n = 0;
fits = false;
pos_x = 0;
pos_y = 0;
rad = 0;
area = 0;
area_history = [];
r = [];
x = [];
y = [];
iters = 0;
iter_history = [];
while n < n_max
    while (not(fits))
        pos_y = rand() * a;
        pos_x = rand() * a;
        rad = rand() * r_max;
        iters = iters + 1;
        if(pos_x + rad < a && pos_x - rad > 0 && pos_y + rad < a && pos_y - rad > 0)
            fits = true;
        end
    end
    
    
    for i = 1:n
        dist = sqrt((pos_x - x(i)) .^ 2 + (pos_y - y(i)).^2);
        rads = rad + r(i);
        if(dist < rads)
            fits = false;
            break
        end
    end
    if(fits)
        
        
        plot_circ(pos_x, pos_y, rad)
        axis equal
        hold on
        n = n + 1;
        iter_history(end+1) = (iters);
        iters = 0;
        area = area + pi * rad.^2;
        x(end+1) = pos_x;
        y(end+1) = pos_y;
        r(end+1) = rad;
        area_history(end+1) = area;
        fprintf(1, ' %s%5d%s%.3g\r ', 'n =',  n, ' S = ', area)
        pause(0.01)
    end
end

figure('Name','Powierzchnia całkowita');

semilogx(1:n, area_history)
xlabel("n")
ylabel("powierzchnia całkowita")
title("wykres powierzchni całkowitej")
saveas(gcf, "powierzchnia.png")

figure('Name', "Średnia liczba losowan");
temp = cumsum(iter_history)./(1:(n_max));
loglog(1:n, temp)
xlabel("n")
ylabel("śrenia liczba losowań")
title("wykres średniej liczby losowań")
saveas(gcf, "losowania.png")

