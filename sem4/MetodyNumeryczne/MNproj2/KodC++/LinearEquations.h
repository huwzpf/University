#pragma once
#include "Matrix.h"
#define MAX_ITERS 100

enum class EqSystemRet
{
	INVALID_SIZE,
	ITERS_EXCEEDED,
	NORM_ACHIEVED,
	OK
};

tuple<Matrix, EqSystemRet, int, double> Jacobi(Matrix& A, Matrix& b, double norm_res);
tuple<Matrix, EqSystemRet, int, double> GS(Matrix& A, Matrix& b, double norm_res);
tuple<Matrix, EqSystemRet, int, double> LU(Matrix& A, Matrix& b);
