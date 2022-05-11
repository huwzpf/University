#include "Matrix.h"


Matrix::Matrix() {
	double* col = new double[1];
	col[0] = 0;
	rows = new double* [1];
	rows[0] = col;
	row_num = 1;
	col_num = 1;
}


Matrix::Matrix(Matrix& other) : row_num{ other.row_num }, col_num{ other.col_num } {
	rows = new double* [row_num];
	for (int i = 0; i < row_num; i++) {
		double* column = new double[col_num];
		for (int j = 0; j < col_num; j++) {
			column[j] = other.rows[i][j];
		}
		rows[i] = column;
	}
}

Matrix::Matrix(Matrix&& other) : row_num{ other.row_num }, col_num{ other.col_num } {
	other.row_num = 0;
	other.col_num = 0;
	rows = other.rows;
	other.rows = nullptr;
}


Matrix::Matrix(int n_rows, int n_colums, double value) {
	rows = new double* [n_rows];
	for (int i = 0; i < n_rows; i++) {
		double* column = new double[n_colums];
		for (int j = 0; j < n_colums; j++) {
			column[j] = value;
		}
		rows[i] = column;
	}
	row_num = n_rows;
	col_num = n_colums;
}

Matrix::Matrix(vector<vector<double>> vec) {
	row_num = vec.size();
	col_num = vec[0].size();

	rows = new double* [row_num];
	for (int i = 0; i < row_num; i++) {

		if (vec[i].size() != col_num)
			throw logic_error("incorrect size");

		double* column = new double[col_num];
		for (int j = 0; j < col_num; j++) {
			column[j] = vec[i][j];
		}
		rows[i] = column;
	}

}

Matrix& Matrix::operator= (Matrix& other) {
	if (row_num != other.row_num || col_num != other.col_num) {
		for (int i = 0; i < row_num; i++) {
			delete[](rows[i]);
		}
		delete[] rows;
		row_num = other.row_num;
		col_num = other.col_num;

	}

	rows = new double* [row_num];
	for (int i = 0; i < row_num; i++) {
		double* column = new double[col_num];
		for (int j = 0; j < col_num; j++) {
			column[j] = other.rows[i][j];
		}
		rows[i] = column;
	}
	
	return *this;
}

Matrix& Matrix::operator= (Matrix&& other){
	row_num = other.row_num;
	col_num = other.col_num;
	other.row_num = 0;
	other.col_num = 0;
	rows = other.rows;
	other.rows = nullptr;
	return *this;
}

Matrix Matrix::operator*(Matrix& other) {
	Matrix ret(row_num, other.get_cols());
	if (col_num != other.get_rows()) {
		throw logic_error("Incorrect size");
	}
	else {
		for (int row = 0; row < row_num; ++row) {
			for (int col = 0; col < other.get_cols(); ++col) {
				for (int i = 0; i < row_num; ++i) {
					ret[row][col] += rows[row][i] * other[i][col];
				}
			}
		}
	}
	return ret;
}

Matrix Matrix::operator*(double scalar) {
	Matrix ret(row_num, col_num);
	for (int row = 0; row < row_num; ++row) {
		for (int col = 0; col < col_num; ++col) {
			ret[row][col] = rows[row][col] * scalar;
		}
	}
	return ret;
}

Matrix Matrix::operator+(Matrix& other) {
	Matrix ret(row_num, col_num);
	if (row_num != other.get_rows() || col_num != other.get_cols()) {
		throw logic_error("Incorrect size");
	}
	else {
		for (int row = 0; row < row_num; ++row) {
			for (int col = 0; col < col_num; ++col) {
				ret[row][col] = rows[row][col] + other[row][col];
			}
		}
	}
	return ret;
}

Matrix Matrix::operator-(Matrix& other) {
	Matrix ret(row_num, col_num);
	if (row_num != other.get_rows() || col_num != other.get_cols()) {
		throw logic_error("Incorrect size");
	}
	else {
		for (int row = 0; row < row_num; ++row) {
			for (int col = 0; col < col_num; ++col) {
				ret[row][col] = rows[row][col] - other[row][col];
			}
		}
	}
	
	return ret;
}

Matrix::operator string() {
	string ret = "";
	for (int i = 0; i < row_num; i++) {
		ret += "{";
		for (int j = 0; j < col_num; j++) {
			ret += to_string(rows[i][j]) + " ";
		}
		ret += "}\n";
	}
	return ret;
}



double Matrix::norm() {
	double ret = 0;
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			ret += rows[i][j] * rows[i][j];
		}
	}
	return sqrt(ret);
}

Matrix::~Matrix() {
	for (int i = 0; i < row_num; i++) {
		delete[](rows[i]);
	}
	delete[] rows;
}