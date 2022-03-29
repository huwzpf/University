clc
clear all
close all
load("Dane_Filtr_Dielektryczny_lab3_MN.mat")

% Gauss
N = size(b, 1);
tic
r = M\b;
czas_1 = toc;
fprintf("\n rozwiązanie metodą bezpośrednią wykonano w czasie %d sekund", czas_1);

max_iter = 10000;

L = tril(M, -1);
U = triu(M, 1);
D = diag(diag(M));


% Jacobi
temp_1_j = D\b;
temp_2_j = -D \ (L + U);

r = ones(N, 1);
res = M * r - b;
tic
iters = 0;
while (norm(res) > 10^(-14) && iters < max_iter && not(isnan(norm(res))))
   iters = iters +1;
   r = temp_2_j * r + temp_1_j;
   res = M * r - b;
end
czas_2 = toc;
if (iters < max_iter && not(isnan(norm(res))))
    fprintf("\n rozwiązanie metodą Jacobiego wykonano w czasie %d sekund i w %d iteracjach", czas_2, iters);
else
     fprintf("\n rozwiązanie metodą Jacobiego nie zbiegło się do rozwiązania po %d iteracjach", iters);
end
% Gaussa-Seidla
iters = 0;
temp_1_g = (D + L)\b;
temp_2_g = -(D + L);

r = ones(N, 1);
res = M * r - b;
tic
while (norm(res) > 10^(-14) && iters < max_iter && not(isnan(norm(res))))
   iters  = iters+1;
   r = temp_2_g\(U * r) + temp_1_g;
   res = M * r - b;
end
czas_2 = toc;
if (iters < max_iter && not(isnan(norm(res))))
    fprintf("\n rozwiązanie metodą Gaussa-Seidla wykonano w czasie %d sekund i w %d iteracjach", czas_2, iters);
else
     fprintf("\n rozwiązanie metodą Gaussa-Seidla nie zbiegło się do rozwiązania po %d iteracjach", iters);
end

