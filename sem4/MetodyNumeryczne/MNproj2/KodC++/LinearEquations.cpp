#include "LinearEquations.h"
#include <stdexcept> 


enum class SolvingMethod
{
	JACOBI,
	GS,
	LU
};


tuple<Matrix, EqSystemRet, int, double> Jacobi_base(Matrix& A, Matrix& b, bool GS, double norm_res) {

	int m = A.get_rows();
	Matrix ret(m, 1);
	EqSystemRet ret_type;
	int i;
		Matrix res;
		vector<double> temp(m);
		double s1, s2;

		for (i = 0; i < MAX_ITERS; ++i) {

			for (int row = 0; row < m; ++row) {

				s1 = 0;
				s2 = 0;

				for (int j = 0; j < row; ++j) {
					if (GS) {
						s1 += A[row][j] * temp[j];
					}
					else {
						s1 += A[row][j] * ret[j][0];
					}
					
				}

				for (int j = row + 1; j < m; ++j) {
					s2 += A[row][j] * ret[j][0];
				}

				temp[row] = (b[row][0] - s1 - s2) / A[row][row];
			}

			for (int row = 0; row < m; ++row) {
				ret[row][0] = temp[row];
			}

			res = (A * ret) - b;

			if (res.norm() <= norm_res) {
				ret_type = EqSystemRet::NORM_ACHIEVED;
				break;
			}
			ret_type = EqSystemRet::ITERS_EXCEEDED;
		}
	return {ret, ret_type, i, res.norm()};
}



pair<Matrix, double> SolveLU(Matrix& A, Matrix& b) {

	EqSystemRet ret_type;
	int m = A.get_rows();
	Matrix ret(m, 1);
	
	
	Matrix L(m, m);
	Matrix U(A);
	Matrix y(m, 1);

	// initialization
	for (int i = 0; i < m; ++i) {
		L[i][i] = 1;
	}

	for (int k = 0; k < m - 1; ++k) {
		for (int j = k + 1; j < m; ++j) {
			L[j][k] = U[j][k] / U[k][k];
			for (int i = k; i < m; ++i) {
				U[j][i] = U[j][i] - L[j][k] * U[k][i];
			}
		}
	}

	double sum = 0;
	for (int i = 0; i < m; ++i) {
		sum = 0;
		for (int k = 0; k < i; k++) {
			sum += L[i][k] * y[k][0];
		}
		y[i][0] = b[i][0] - sum;
	}

	for (int i = m - 1; i >= 0; --i) {
		sum = 0;
		for (int k = i + 1; k < m; k++) {
			sum += U[i][k] * ret[k][0];
		}
		ret[i][0] = (y[i][0] - sum) / U[i][i];
	}
	Matrix res = (A * ret) - b;
	return { ret, res.norm() };
}

tuple<Matrix, EqSystemRet, int, double> SolveEq(Matrix& A, Matrix& b, SolvingMethod Method, double norm_res = 0) {

	if ((A.get_rows() != A.get_cols()) || (b.get_cols() != 1) || (b.get_rows() != A.get_rows())) {
		Matrix ret(1,1);
		return {ret, EqSystemRet::INVALID_SIZE, 0, 0};
	}
	else {
		if (Method == SolvingMethod::JACOBI) {
			return  Jacobi_base(A, b, false, norm_res);
		}
		else if (Method == SolvingMethod::GS) {
			return  Jacobi_base(A, b, true, norm_res);
		}
		else {
			auto ret = SolveLU(A, b);
			return  { ret.first, EqSystemRet::OK, 0, ret.second};
		}
	}
}

tuple<Matrix, EqSystemRet, int, double> GS(Matrix& A, Matrix& b, double norm_res) {
	return SolveEq(A, b, SolvingMethod::GS, norm_res);
}
tuple<Matrix, EqSystemRet, int, double> Jacobi(Matrix& A, Matrix& b, double norm_res) {
	return SolveEq(A, b, SolvingMethod::JACOBI, norm_res);
}
tuple<Matrix, EqSystemRet, int, double> LU(Matrix& A, Matrix& b) {
	return SolveEq(A, b, SolvingMethod::LU);
}

