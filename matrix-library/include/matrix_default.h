#pragma once
#include "macros.h"
#include "matrix_iface.h"
#include <memory>

class matrix_default : public matrix_iface
{
public:
	matrix_iface* Tanh(const matrix_iface& A) override;
	matrix_iface* Tanh_D(const matrix_iface& A) override;

	matrix_iface* Sigmoid(const matrix_iface& A) override;

	matrix_iface* Sigmoid_D(const matrix_iface& A) override;

	matrix_proxy getPrx(int i, int j) const override;
	matrix_dtype getAt(int i, int j) const override;
	void setAt(int i, int j, matrix_dtype& x) const override;
private:
	matrix_default(int r, int c, matrix_dtype def = 0);
public:
	matrix_default() = default;
	std::shared_ptr<matrix_dtype> _matrix;
	void create(int r, int c, matrix_dtype defaultValues) override;
	matrix_dtype* getArr() const override;
	~matrix_default() override = default;
	bool isEmpty() const override;
	matrix_iface* shallow_copy(const matrix_iface& x) override;
	matrix_iface* deep_copy(const matrix_iface& x) override;
	void cpy_ctor(const matrix_iface& cp) override;
	void memcpy(matrix_iface& dst) const override;
	void move_ctor(matrix_iface&& mv) override;
	void move(matrix_iface& dst) override;
	matrix_iface* Add(const matrix_iface& A, const matrix_iface& B) override;
	matrix_iface* Add(const matrix_iface& A, matrix_dtype x) override;
	matrix_iface* Subtract(const matrix_iface& A, const matrix_iface& B) override;
	matrix_iface* Multiply(const matrix_iface& A, const matrix_iface& B) override;
	matrix_iface* Multiply(const matrix_iface& A, matrix_dtype x) override;
	matrix_iface* Multiply_BT(const matrix_iface& A, const matrix_iface& B) override;
	matrix_iface* Dotprod(const matrix_iface& A, const matrix_iface& B) override;
	matrix_dtype Sum(const matrix_iface& A) override;
	matrix_iface* Transpose(const matrix_iface& A) override;
	void Add_inp(matrix_iface& A, const matrix_iface& B) override;
	void Add_inp(matrix_iface& A, matrix_dtype x) override;
	void Multiply_inp(matrix_iface& A, matrix_dtype x) override;
	void Subtract_inp(matrix_iface& A, const matrix_iface& B) override;
	void Dotprod_inp(matrix_iface& A, const matrix_iface& B) override;
	void Pow_inp(matrix_iface& A, int power) override;
	void Transpose_inp(matrix_iface& A) override;
};

