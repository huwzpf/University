clc
clear all
close all
diary('log_184297_lab3')
% odpowiednie fragmenty kodu mo¿na wykonaæ poprzez znazaczenie i wciœniêcie F9
% komentowanie/ odkomentowywanie: ctrl+r / ctrl+t

% Zadanie A
%------------------
N = 10;
density = 3; % parametr decyduj¹cy o gestosci polaczen miedzy stronami
[Edges] = generate_network(N, density);
%-----------------

% Zadanie B
%------------------
% generacja macierzy I, A, B i wektora b
% ...
  % macierze A, B i I musz¹ byæ przechowywane w formacie sparse (rzadkim)
 d=0.85;
B = sparse(Edges(2, :), Edges(1, :), ones(1, size(Edges, 2)), N, N);
I = speye(N);
A = sparse(1:N, 1:N, 1./(transpose(B) * ones(N, 1)));
b = ((1-d)/N)*ones(N, 1);

save zadB_184297 A B I b
%-----------------
M = I - d * B * A;
r = M\b;
save zadC_184297 r

% Zadanie D
%------------------
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
tic
M = I - d * B * A;
r = M\b;

czas_Gauss(i) = toc;
end

plot(N, czas_Gauss)
xlabel("Liczba stron w sieci")
ylabel("Czas obliczeñ [s]")
title("wykres czasu obliczeñ od liczby stron w sieci dla metody bezpoœredniej")
saveas(gcf,'zadD_184297.png')
%------------------

% Zadanie E - w osobnym pliku
run("Jacobi.m");
% Zadanie F - w osobnym pliku
run("GS.m");
% zadanie G - w osobnym pliku
run("ZadG.m");


