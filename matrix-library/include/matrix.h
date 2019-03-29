#pragma once
#include "macros.h"
#include "matrix_iface.h"
#include <vector>

class matrix
{
public:
	enum MATRIX_ACCEL
	{
		DEFAULT,
		MULTI_CPU,
		GPU
	};
private:
	std::shared_ptr<matrix_iface> ths;
	static std::shared_ptr<matrix_iface> createMatrix(MATRIX_ACCEL &acceleration);
	matrix(matrix_iface* x);
public:
	MATRIX_ACCEL acceleration;
	matrix(MATRIX_ACCEL accel = MULTI_CPU);
	matrix(int r, int c, matrix_dtype def = 0, MATRIX_ACCEL accel = MULTI_CPU);
	matrix(const matrix& cp);
	matrix(const std::vector<std::vector<matrix_dtype>>& in, MATRIX_ACCEL accel = MULTI_CPU);
	void changeAcceleration(MATRIX_ACCEL newAccel);
	matrix_proxy getPrx(int i, int j);
	const matrix_proxy getPrx(int i, int j) const;
	bool isEmpty() const;
	matrix_dtype getAt(int i, int j) const;
	matrix_dtype* getArr();
	int cols() const { return ths ? ths->cols() : 0; }
	int rows() const { return ths ? ths->rows() : 0; }
	void reshape(int r, int c);
	matrix& operator=(const matrix& cp);
	matrix(matrix&& mv);
	matrix& operator=(matrix&& mv);
	matrix& operator+=(const matrix& x);
	matrix& operator+=(const matrix_dtype& x);
	matrix& operator-=(const matrix& x);
	matrix& operator*=(const matrix_dtype& x);
	matrix operator+(const matrix& x) const;
	matrix operator+(const matrix_dtype& x) const;
	matrix operator-(const matrix& x) const;
	matrix operator*(const matrix& x) const;
	matrix operator*(const matrix_dtype& x) const;
	static matrix shallow_copy(const matrix& x);
	static matrix deep_copy(const matrix& x);
	static matrix Add(const matrix& A, const matrix& B);
	static matrix Add(const matrix& A, matrix_dtype x);
	static matrix Subtract(const matrix& A, const matrix& B);
	static matrix Multiply(const matrix& A, const matrix& B);
	static matrix Multiply(const matrix& A, const matrix_dtype& x);
	static matrix Multiply_BT(const matrix& A, const matrix& B);
	static matrix Hadamard(const matrix& A, const matrix& B);
	static matrix_dtype Sum(const matrix& A);
	static matrix Transpose(const matrix& A);
	static void Add_inp(matrix& A, const matrix& B);
	static void Add_inp(matrix& A, const matrix_dtype& x);
	static void Multiply_inp(matrix& A, const matrix_dtype& x);
	static void Subtract_inp(matrix& A, const matrix& B);
	static void Hadamard_inp(matrix& A, const matrix& B);
	static void Pow_inp(matrix& A, int power);
	static void Transpose_inp(matrix& A);
	static matrix Tanh(matrix& A);
	static matrix Tanh_D(const matrix& A);
	static matrix Sigmoid(matrix& A);
	static matrix Sigmoid_D(const matrix& A);
};


