clc
clear all
close all

% ZAD 2.1

[xvect, xdif, fx, it_cnt] = bisect(1,60000,1e-3,@compute_time);
semilogy(xdif);
xlabel("numer iteracji")
ylabel("różnica między kolejnymi wartościami")
title(["wykres numeru iteracji od różnicy między liczbą parametrów wejściowych", ...
    "w kolejnych iteracjach - metoda bisekcji"])
saveas(gcf, 'czas_bisekcja_0.png')
semilogy(xvect);
xlabel("numer iteracji")
ylabel("liczba parametrów wejściowych")
title(["wykres numeru iteracji od liczby parametrów wejściowych w tej iteracji", ...
    " - metoda bisekcji"])
saveas(gcf, 'czas_bisekcja_1.png')

[xvect, xdif, fx, it_cnt] = secant(1,60000,1e-3,@compute_time);
semilogy(xdif);
xlabel("numer iteracji")
ylabel("różnica między kolejnymi wartościami")
title(["wykres numeru iteracji od różnicy między liczbą parametrów wejściowych", ...
    "w kolejnych iteracjach - metoda siecznych"])
saveas(gcf, 'czas_sieczne_0.png')
semilogy(xvect);
xlabel("numer iteracji")
ylabel("liczba parametrów wejściowych")
title(["wykres numeru iteracji od liczby parametrów wejściowych w tej iteracji", ...
    " - metoda siecznych"])
saveas(gcf, 'czas_sieczne_1.png')

% ZAD 2.2

[xvect, xdif, fx, it_cnt] = bisect(0,50,1e-12,@compute_impedance);
semilogy(xdif);
xlabel("numer iteracji")
ylabel("różnica między kolejnymi wartościami")
title(["wykres numeru iteracji od różnicy międzu wartościami częstości kątowej", ...
    "w kolejnych iteracjach - metoda bisekcji"])
saveas(gcf, 'impedancja_bisekcja_0.png')
semilogy(xvect);
xlabel("numer iteracji")
ylabel("wartość częstości kątowej")
title(["wykres numeru iteracji od wartości częstości kątowej w tej iteracji", ...
    " - metoda bisekcji"])
saveas(gcf, 'impedancja_bisekcja_1.png')

[xvect, xdif, fx, it_cnt] = secant(0,50,1e-12,@compute_impedance);
semilogy(xdif);
xlabel("numer iteracji")
ylabel("różnica między kolejnymi wartościami")
title(["wykres numeru iteracji od różnicy międzu wartościami częstości kątowej", ...
    "w kolejnych iteracjach - metoda siecznych"])
saveas(gcf, 'impedancja_sieczne_0.png')
semilogy(xvect);
xlabel("numer iteracji")
ylabel("wartość częstości kątowej")
title(["wykres numeru iteracji od wartości częstości kątowej w tej iteracji", ...
    " - metoda siecznych"])
saveas(gcf, 'impedancja_sieczne_1.png')

% ZAD 2.3



[xvect, xdif, fx, it_cnt] = bisect(0,50,1e-12,@compute_impedance);
semilogy(xdif);
xlabel("numer iteracji")
ylabel("różnica między kolejnymi czasami")
title(["wykres numeru iteracji od różnicy między wartościami czasu", ...
    "w kolejnych iteracjach - metoda bisekcji"])
saveas(gcf, 'predkosc_bisekcja_0.png')
semilogy(xvect);
xlabel("numer iteracji")
ylabel("czas")
title(["wykres numeru iteracji od wartości czasu w tej iteracji", ...
    " - metoda bisekcji"])
saveas(gcf, 'predkosc_bisekcja_1.png')

[xvect, xdif, fx, it_cnt] = secant(0,50,1e-12,@compute_impedance);
semilogy(xdif);
xlabel("numer iteracji")
ylabel("różnica między kolejnymi czasami")
title(["wykres numeru iteracji od różnicy między wartościami czasu", ...
    "w kolejnych iteracjach - metoda siecznych"])
saveas(gcf, 'predkosc_sieczne_0.png')
semilogy(xvect);
xlabel("numer iteracji")
ylabel("czas")
title(["wykres numeru iteracji od wartości czasu w tej iteracji", ...
    " - metoda siecznych"])
saveas(gcf, 'predkosc_sieczne_1.png')


% ZAD 3
options = optimset('Display', 'iter');
fzero(@tan, 6, options)
fzero(@tan, 4.5, options)
