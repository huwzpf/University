function [] = zad1()
K = [5, 15, 25, 35];
[XX,YY]=meshgrid(linspace(0,100,101),linspace(0,100,101));
for i = 1:4
    [x, y, f] = lazik(K(i));

    subplot(2,2,1)
    plot(x,y,'-o','linewidth',1);
    grid on
    xlabel("x[m]")
    ylabel("y[m]")
    title(["Tor ruchu łazika"])

    subplot(2,2,2)
    plot3(x,y,f,'o');
    grid on
    xlabel("x[m]")
    ylabel("y[m]")
    zlabel("f(x, y)")
    title(["Zebrane wartości próbek"])
    
    subplot(2,2,3)
    [p]=polyfit2d(x,y,f);
    [FF]=polyval2d(XX,YY,p);
    surf(XX,YY,FF);
    shading flat;
    xlabel("x[m]")
    ylabel("y[m]")
    zlabel("f(x, y)")
    title(["Interpolacja wielomianowa"])

    subplot(2,2,4)
    [p]=trygfit2d(x,y,f);
    [FF]=trygval2d(XX,YY,p);
    surf(XX,YY,FF);
    shading flat;
    xlabel("x[m]")
    ylabel("y[m]")
    zlabel("f(x, y)")
    title(["Interpolacja trygonometryczna"])
    saveas(gcf, i+"_zadanie1.png")
end
end