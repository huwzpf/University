#pragma once
#include <vector>
#include<string>
#include<memory>
#include <tuple>
#include <iostream>

using namespace std;

class Matrix
{
	double** rows;
	int row_num, col_num;
public:
	Matrix();
	Matrix(Matrix& other);
	Matrix(Matrix&& other);
	Matrix(int n_rows, int n_colums, double value = 0);
	Matrix(vector<vector<double>> vec);

	
	double* operator[] (size_t idx) {
		return rows[idx];
	}

	Matrix operator* (Matrix& other);
	Matrix operator* (double scalar);
	Matrix operator- (Matrix& other);
	Matrix operator+ (Matrix& other);
	Matrix& operator= (Matrix& other);
	Matrix& operator= (Matrix&& other);

	operator string();

	int get_rows() const {
		return row_num;
	}

	int get_cols() const {
		return col_num;
	}

	double norm();

	~Matrix();
};
