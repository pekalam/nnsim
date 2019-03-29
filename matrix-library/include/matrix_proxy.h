#pragma once
#include "datadef.h"

class matrix_iface;

class matrix_proxy
{
private:
	const matrix_iface *mat;
	int i, j;
	matrix_dtype val;
public:
	matrix_proxy(const matrix_iface* mat, int i, int j);
	operator matrix_dtype() const;

	matrix_proxy& operator+=(const matrix_proxy& x);
	matrix_proxy& operator-=(const matrix_proxy& x);
	matrix_proxy& operator*=(const matrix_proxy& x);
	matrix_proxy& operator=(const matrix_proxy& x);
	matrix_proxy& operator=(matrix_dtype x);
	matrix_proxy& operator+=(matrix_dtype a);
	matrix_proxy& operator-=(matrix_dtype a);
	matrix_proxy& operator*=(matrix_dtype a);
	matrix_dtype operator+(matrix_dtype a) const;
	matrix_dtype operator-(matrix_dtype a) const;
	matrix_dtype operator*(matrix_dtype a) const;
	matrix_dtype operator/(matrix_dtype a) const;
};

