#include "LinearEquations.h"
#include <chrono>
#include <fstream>

using namespace std;

Matrix& constructA(double a1, double a2, double a3, int N);

Matrix& constructB(double f, int N);

void test_for_given_a_b(Matrix& a, Matrix& b);

void test_for_given_size(int e, int i, int f, std::ofstream& file);

int main()
{

    /*
            184297 - > c = 9, d = 7, e = 2, f = 4
            N = 9cd = 997
    */
    
    int N = 997, e = 2, f = 4;
    Matrix a, b;
    
    //ZADANIE A

    a = move(constructA(5 + e, -1, -1, N));
    b = move(constructB(f, N));
    cout << string(a) << endl << string(b) << endl;
    // ZADANIE B

    cout << "\n\n1) a1 = " << 5 + e << ", a2 = -1, a3 = -1, N = " << N << endl;
    test_for_given_a_b(a, b);
    a = move(constructA(3, -1, -1, N));

    //ZADANIE C+ D

    cout << "\n\n2) a1 = 3, a2 = -1, a3 = -1, N = " << N << endl;
    test_for_given_a_b(a, b);


    
    //GENEROWANIE DANYCH DO ZADANIA E

    int start_N = 100, end_N = 5100, step_N = 200;
    ofstream file;
    file.open("data.csv");

    file << "n,Jacobi,GS\n";
    for (int i = start_N; i <= end_N; i += step_N) {

        test_for_given_size(e, f, i, file);
        cout << "\niter " << i << " done\n";
    }
    file.close();
    
    cout << "\nDONE\n";
    return 0;
}

Matrix& constructA(double a1, double a2, double a3, int N) {
    Matrix& ret = *new Matrix(N, N);
    for (int i = 0; i < N; i++) {
        if (i > 1) ret[i][i - 2] = a3;
        if (i > 0) ret[i][i - 1] = a2;
        ret[i][i] = a1;
        if (i < N - 1) ret[i][i + 1] = a2;
        if (i < N - 2) ret[i][i + 2] = a3;
    }

    return ret;
}
Matrix& constructB(double f, int N) {
    Matrix& ret = *new Matrix(N, 1);
    for (int i = 0; i < N; i++) {
        ret[i][0] = sin((i + 1) * (f + 1));
    }

    return ret;
}

void test_for_given_size(int e, int f, int size, ofstream& file)
{
    chrono::steady_clock::time_point start, stop;
    chrono::milliseconds duration;

    double n_res = pow(10, -9);

    Matrix x, a, b;
    EqSystemRet ret;
    int iters;
    double norm;

    a = move(constructA(5 + e, -1, -1, size));
    b = move(constructB(f, size));

    file << size << ",";


    start = chrono::high_resolution_clock::now();

    tie(x, ret, iters, norm) = Jacobi(a, b, n_res);

    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    if (ret != EqSystemRet::ITERS_EXCEEDED) {
        file << duration.count() << ",";
    }
    else {
        file << "X,";
    }

    start = chrono::high_resolution_clock::now();

    tie(x, ret, iters, norm) = GS(a, b, n_res);

    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    if (ret != EqSystemRet::ITERS_EXCEEDED) {
        file << duration.count();
    }
    else {
        file << "X";
    }
    file << "\n";
}

void test_for_given_a_b(Matrix& a, Matrix& b)
{

    double n_res = pow(10, -9);

    Matrix x;
    EqSystemRet ret;
    int iters;
    double norm;

    chrono::steady_clock::time_point start, stop;
    chrono::milliseconds duration;

    start = chrono::high_resolution_clock::now();

    tie(x, ret, iters, norm) = Jacobi(a, b, n_res);

    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

   
    if (ret != EqSystemRet::ITERS_EXCEEDED) {
        cout << "jacobi done in: " << iters << " iters and " << duration.count() << " ms\n" << "norm from residuum: " << norm << endl;
    }
    else {
        cout << "jacobi MAX_ITERS= " << iters << " exceeded in " << duration.count() << " ms\n" << "norm from residuum: " << norm << endl;
    }

    start = chrono::high_resolution_clock::now();

    tie(x, ret, iters, norm) = GS(a, b, n_res);

    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    if (ret != EqSystemRet::ITERS_EXCEEDED) {
        cout << "gs done in: " << iters << " iters and " << duration.count() << " ms\n" << "norm from residuum: " << norm << endl;
    }
    else {
        cout << "gs MAX_ITERS= " << iters << " exceeded in " << duration.count() << " ms\n" << "norm from residuum: " << norm << endl;
    }

    start = chrono::high_resolution_clock::now();

    tie(x, ret, iters, norm) = LU(a, b);

    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "LU done in: " << duration.count() << " ms\n" << "norm from residuum: " << norm << endl;

}

