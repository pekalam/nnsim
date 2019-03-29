#pragma once

#include <memory>
#include "matrix_proxy.h"
#include "datadef.h"


class matrix;

class matrix_iface
{
protected:
	int rows_ = 0;
	int cols_ = 0;
public:
	enum ACCEL
	{
		DEFAULT, AF_GPU, ARMA_CPU
	};
	virtual matrix_proxy getPrx(int i, int j) const = 0;
	virtual ACCEL actype() const { return DEFAULT; };
	virtual void create(int r, int c, matrix_dtype defaultValues = 0) = 0;
	int rows() const { return rows_; }
	int cols() const { return cols_; }
	virtual matrix_dtype* getArr() const = 0;
	virtual matrix_dtype getAt(int i, int j) const = 0;
	virtual void setAt(int i, int j, matrix_dtype& x) const = 0;
	virtual void memcpy(matrix_iface& dst) const = 0;
	virtual void move(matrix_iface &dst) = 0;
	virtual ~matrix_iface() = default;
	virtual bool isEmpty() const = 0;
	virtual matrix_iface* shallow_copy(const matrix_iface& x) = 0;
	virtual matrix_iface* deep_copy(const matrix_iface& x) = 0;
	virtual void cpy_ctor(const matrix_iface &cp) = 0;
	virtual void move_ctor(matrix_iface &&mv) = 0;
	virtual matrix_iface* Add(const matrix_iface &A, const matrix_iface &B) = 0;
	virtual matrix_iface* Add(const matrix_iface &A, matrix_dtype x) = 0;
	virtual matrix_iface* Subtract(const matrix_iface &A, const matrix_iface &B) = 0;
	virtual matrix_iface* Multiply(const matrix_iface &A, const matrix_iface &B) = 0;
	virtual matrix_iface* Multiply(const matrix_iface &A, matrix_dtype x) = 0;
	virtual matrix_iface* Multiply_BT(const matrix_iface &A, const matrix_iface &B) = 0;
	virtual matrix_iface* Dotprod(const matrix_iface &A, const matrix_iface &B) = 0;
	virtual matrix_dtype Sum(const matrix_iface &A) = 0;
	virtual matrix_iface* Transpose(const matrix_iface &A) = 0;
	virtual void Add_inp(matrix_iface& A, const matrix_iface& B) = 0;
	virtual void Add_inp(matrix_iface& A, matrix_dtype x) = 0;
	virtual void Multiply_inp(matrix_iface& A, matrix_dtype x) = 0;
	virtual void Subtract_inp(matrix_iface& A, const matrix_iface& B) = 0;
	virtual void Dotprod_inp(matrix_iface& A, const matrix_iface& B) = 0;
	virtual void Pow_inp(matrix_iface& A, int power) = 0;
	virtual void Transpose_inp(matrix_iface &A) = 0;
	virtual matrix_iface* Tanh(const matrix_iface& A) = 0;
	virtual matrix_iface* Tanh_D(const matrix_iface& A) = 0;
	virtual matrix_iface* Sigmoid(const matrix_iface& A) = 0;
	virtual matrix_iface* Sigmoid_D(const matrix_iface& A) = 0;
};


