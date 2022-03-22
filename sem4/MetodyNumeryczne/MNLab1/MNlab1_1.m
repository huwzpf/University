pages = 7;
d = 0.85;
Edges = sparse(2, 15);
Edges(:, 1) = [1, 4];
Edges(:, 2) = [1, 6];
Edges(:, 3) = [2, 3];
Edges(:, 4) = [2, 4];
Edges(:, 5) = [2, 5];
Edges(:, 6) = [3, 5];
Edges(:, 7) = [3, 6];
Edges(:, 8) = [3, 7];
Edges(:, 9) = [4, 5];
Edges(:, 10) = [4, 6];
Edges(:, 11) = [5, 4];
Edges(:, 12) = [5, 6];
Edges(:, 13) = [6, 4];
Edges(:, 14) = [6, 7];
Edges(:, 15) = [7, 6];

I = speye(pages)
B = sparse(Edges(2, :), Edges(1, :), ones(1, 15), pages, pages);
b = ((1-d)/pages)*ones(pages, 1);
% transpose(B) * ones(pages, 1) zwraca wektor gdzie n-ty wyraz jest sumą
% n-tego rzędu transpose(B) więc n-tej kolumny B
A = sparse(1:pages, 1:pages, 1./(transpose(B) * ones(pages, 1)));

M = I - d * B * A;
% Mr = b -> r = M^-1 b
r = inv(M) * b;
bar(r);
