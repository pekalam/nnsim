#include "matrix_proxy.h"
#include "matrix_iface.h"


matrix_proxy::matrix_proxy(const matrix_iface* mat, int i, int j): i(i), j(j)
{
	this->mat = mat;
	val = mat->getAt(i, j);
}

matrix_proxy::operator matrix_dtype() const
{
	return val;
}

matrix_proxy& matrix_proxy::operator+=(const matrix_proxy& x)
{
	matrix_dtype r = val + x.val;
	mat->setAt(i, j, r);
	return *this;
}

matrix_proxy& matrix_proxy::operator-=(const matrix_proxy& x)
{
	matrix_dtype r = val - x.val;
	mat->setAt(i, j, r);
	return *this;
}

matrix_proxy& matrix_proxy::operator*=(const matrix_proxy& x)
{
	matrix_dtype r = val * x.val;
	mat->setAt(i, j, r);
	return *this;
}

matrix_proxy& matrix_proxy::operator=(const matrix_proxy& x)
{
	val = x.val;
	mat->setAt(i, j, val);
	return *this;
}


matrix_proxy& matrix_proxy::operator-=(matrix_dtype a)
{
	matrix_dtype r = val - a;
	mat->setAt(i, j, r);
	return *this;
}

matrix_proxy& matrix_proxy::operator*=(matrix_dtype a)
{
	matrix_dtype r = val * a;
	mat->setAt(i, j, r);
	return *this;
}

matrix_dtype matrix_proxy::operator+(matrix_dtype a) const
{
	return val + a;
}

matrix_dtype matrix_proxy::operator-(matrix_dtype a) const
{
	return val - a;
}


matrix_dtype matrix_proxy::operator*(matrix_dtype a) const
{
	return val * a;
}

matrix_dtype matrix_proxy::operator/(matrix_dtype a) const
{
	return val / a;
}



matrix_proxy& matrix_proxy::operator+=(matrix_dtype a)
{
	matrix_dtype r = val + a;
	mat->setAt(i, j, r);
	return *this;
}

matrix_proxy& matrix_proxy::operator=(matrix_dtype x)
{
	mat->setAt(i, j, x);
	val = x;
	return *this;
}
