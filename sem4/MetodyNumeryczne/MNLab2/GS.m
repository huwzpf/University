clc
clear all
close all
density = 10;
d = 0.85;
N = [500, 1000, 3000, 6000, 12000];



    
for i = 1:5

[Edges] = generate_network(N(i), density);

B = sparse(Edges(2, :), Edges(1, :), ones(1, size(Edges, 2)), N(i), N(i));
I = speye(N(i));
A = sparse(1:N(i), 1:N(i), 1./(transpose(B) * ones(N(i), 1)));
b = ((1-d)/N(i))*ones(N(i), 1);

M = I - d * B * A;

iters(i) = 0;
L = tril(M, -1);
U = triu(M, 1);
D = diag(diag(M));
temp_1 = (D + L)\b;
temp_2 = -(D + L);
r = ones(N(i), 1);
res = M * r - b;
tic
while (norm(res) > 10^(-14))
   iters(i) = iters(i)+1;
   r = temp_2\(U * r) + temp_1;
   res = M * r - b;
    if i == 2 
    norm_res(iters(i)) = norm(res);
   end 
end
czas_GS(i) = toc;
end

plot(N, czas_GS)
xlabel("Liczba stron w sieci")
ylabel("Czas obliczeń [s]")
title("wykres czasu obliczeń od liczby stron w sieci dla metody Gaussa-Seidla")
saveas(gcf,'zadF_184297_1.png')

plot(N, iters)
xlabel("Liczba stron w sieci")
ylabel("Liczba iteracji")
title("wykres liczby iteracji potrzebnych od osiągnięcia rozwiązania", "od liczby stron w sieci dla metody Gaussa-Seidla")
saveas(gcf,'zadF_184297_2.png')

semilogy(norm_res)
ylabel("norma z residuum")
xlabel("iteracja")
title("wykres normy z resiuum od numeru iteracji dla metody Gaussa-Seidla dla N = 1000")
saveas(gcf,'zadF_184297_3.png')