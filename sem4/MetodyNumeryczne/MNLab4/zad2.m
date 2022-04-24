function [] = zad2()
div_poly = [];
div_trig = [];
[XX,YY]=meshgrid(linspace(0,100,101),linspace(0,100,101));

FF_K_prev_poly = 0;

FF_K_prev_trig = 0;

for K = 5:45
    [x, y, f] = lazik(K);

    [p]=polyfit2d(x,y,f);
    [FF_K_poly]=polyval2d(XX,YY,p);
    div_poly(end + 1) = max(max(abs(FF_K_poly - FF_K_prev_poly)));
    FF_K_prev_poly = FF_K_poly;

    [p]=trygfit2d(x,y,f);
    [FF_K_trig]=trygval2d(XX,YY,p);
    div_trig(end + 1) = max(max(abs(FF_K_trig - FF_K_prev_trig)));
    FF_K_prev_trig = FF_K_trig;
    
end
    subplot(2,1,1)
    plot(5:45,div_trig,'-o','linewidth',1);
    grid on
    xlabel("K")
    ylabel("div(K)")
    title(["Maksymalna wartość różnicy interpolowanych funkcji (interpolacja trygonometryczna)"])


    subplot(2,1,2)
    plot(5:45,div_poly,'-o','linewidth',1);
    grid on
    xlabel("K")
    ylabel("div(K)")
    title(["Maksymalna wartość różnicy interpolowanych funkcji (interpolacja wielomianowa)"])
    saveas(gcf, "zadanie2.png")
end