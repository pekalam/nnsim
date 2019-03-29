#include "matrix.h"
#include "matrix_iface.h"
#include "matrix_default.h"
#include "matrix_af.h"
#include "matrix_proxy.h" 
#include "matrix_arma.h"

std::shared_ptr<matrix_iface> matrix::createMatrix(MATRIX_ACCEL &acceleration)
{
#ifdef MAT_ARRAYFIRE
	if (acceleration == GPU)
	{
		std::shared_ptr<matrix_iface> p(new matrix_af);
		return p;
	}
#endif
#ifdef MAT_ARMADILLO
	if(acceleration == MULTI_CPU)
	{
		std::shared_ptr<matrix_iface> p(new matrix_arma);
		return p;
	}
#endif
	std::shared_ptr<matrix_iface> p(new matrix_default);
	return p;
}

matrix::matrix(MATRIX_ACCEL accel)
{
	matrix(0, 0, 0, accel);
}

matrix::matrix(int r, int c, matrix_dtype def, MATRIX_ACCEL accel)
{
	acceleration = accel;
	ths = std::move(createMatrix(acceleration));

	ths.get()->create(r, c, def);
}

matrix::matrix(matrix_iface* x)
{
	std::shared_ptr<matrix_iface> p(x);
	ths = std::move(p);
	if (x->actype() == matrix_iface::DEFAULT)
		acceleration = DEFAULT;
#ifdef MAT_ARRAYFIRE
	if (x->actype() == matrix_iface::AF_GPU)
		acceleration = GPU;
#endif
#ifdef MAT_ARMADILLO
	if (x->actype() == matrix_iface::ARMA_CPU)
		acceleration = MULTI_CPU;
#endif
}

matrix::matrix(const matrix& cp)
{
	if (&cp == this)
		throw std::exception();
	acceleration = cp.acceleration;
	ths = std::move(createMatrix(acceleration));
	ths.get()->cpy_ctor(*cp.ths.get());
}

matrix::matrix(const std::vector<std::vector<matrix_dtype>>& in, MATRIX_ACCEL accel)
{
	if (in.size() == 0)
		throw std::exception();
	const int rows = in.size();
	const int cols = in.at(0).size();
	acceleration = accel;
	ths = std::move(createMatrix(acceleration));
	ths.get()->create(rows, cols);
	int i = 0;
	int j = 0;
	for(auto& col : in)
	{
		if (col.size() != cols)
			throw std::exception();
		j = 0;
		for(auto num : col)
		{
			ths.get()->setAt(i, j, num);
			j++;
		}
		i++;
	}
	
	
}

void matrix::changeAcceleration(MATRIX_ACCEL newAccel)
{
	if (newAccel == acceleration)
		return;
	acceleration = newAccel;
#ifdef MAT_ARRAYFIRE
	if(acceleration == GPU)
	{
		std::shared_ptr<matrix_iface> p(new matrix_af);
		p->rows_ = rows_;
		p->cols_ = cols_;
		p->cpy_ctor(*ths.get());
		ths = std::move(p);
	}
#endif
#ifdef MAT_ARMADILLO
	if(acceleration == MULTI_CPU)
	{
		std::shared_ptr<matrix_iface> p(new matrix_arma);
		p->cpy_ctor(*ths.get());
		ths = std::move(p);
	}
#endif
	if(acceleration == DEFAULT)
	{
		std::shared_ptr<matrix_iface> p(new matrix_default);
		p->cpy_ctor(*ths.get());
		ths = std::move(p);
	}
}

void matrix::reshape(int r, int c)
{
	ths = std::move(createMatrix(acceleration));
	ths.get()->create(r, c, 0);
}

matrix& matrix::operator=(const matrix& cp)
{
	if (&cp == this)
		return *this;
	acceleration = cp.acceleration;
	ths = std::move(createMatrix(acceleration));
	ths.get()->cpy_ctor(*cp.ths.get());
	return *this;
}

matrix::matrix(matrix&& mv)
{
	if (&mv == this)
		return;
	ths = mv.ths;
	acceleration = mv.acceleration;
	mv.ths.reset();
}

matrix& matrix::operator=(matrix&& mv)
{
	if (&mv == this)
		return *this;
	ths = mv.ths;
	acceleration = mv.acceleration;
	mv.ths.reset();
	return *this;
}

matrix_proxy matrix::getPrx(int i, int j) 
{
	return ths->getPrx(i, j);
}

const matrix_proxy matrix::getPrx(int i, int j) const
{
	return ths->getPrx(i, j);
}

bool matrix::isEmpty() const
{
	return ths == nullptr || ths->isEmpty();
}

matrix_dtype matrix::getAt(int i, int j) const
{
	return ths.get()->getArr()[i * (ths.get()->cols()) + j];
}

matrix_dtype* matrix::getArr()
{
	return ths->getArr();
}

matrix& matrix::operator+=(const matrix& x)
{
	Add_inp(*this, x);
	return *this;
}

matrix& matrix::operator+=(const matrix_dtype& x)
{
	Add_inp(*this, x);
	return *this;
}

matrix matrix::operator+(const matrix& x) const
{
	return std::move(Add(*this, x));
}

matrix matrix::operator+(const matrix_dtype& x) const
{
	return std::move(Add(*this, x));
}

matrix& matrix::operator-=(const matrix& x)
{
	Subtract_inp(*this, x);
	return *this;
}

matrix matrix::operator-(const matrix& x) const
{
	return std::move(Subtract(*this, x));
}

matrix& matrix::operator*=(const matrix_dtype& x)
{
	Multiply_inp(*this, x);
	return *this;
}

matrix matrix::operator*(const matrix& x) const
{
	return std::move(Multiply(*this, x));
}

matrix matrix::operator*(const matrix_dtype& x) const
{
	return std::move(Multiply(*this, x));
}

matrix matrix::shallow_copy(const matrix& x)
{
	matrix r;
	r.ths = x.ths;
	r.acceleration = x.acceleration;
	return r;
}

matrix matrix::deep_copy(const matrix& x)
{
	if (!x.isEmpty())
	{
		matrix r(x.rows(), x.cols());
		r.acceleration = x.acceleration;
		std::shared_ptr<matrix_iface> p(new matrix_default);
		r.ths = std::move(p);
		r.ths->create(x.rows(), x.cols());
		
		x.ths->memcpy(*r.ths);
		return r;
	}
	else
		return matrix(0, 0);
}

matrix matrix::Add(const matrix& A, const matrix& B)
{
	return matrix(A.ths.get()->Add(*A.ths.get(), *B.ths.get()));
}

matrix matrix::Add(const matrix& A, matrix_dtype x)
{
	return matrix(A.ths.get()->Add(*A.ths.get(), x));
}

matrix matrix::Subtract(const matrix& A, const matrix& B)
{
	return matrix(A.ths.get()->Subtract(*A.ths.get(), *B.ths.get()));
}

matrix matrix::Multiply(const matrix& A, const matrix& B)
{
	return matrix(A.ths.get()->Multiply(*A.ths.get(), *B.ths.get()));
}

matrix matrix::Multiply(const matrix& A, const matrix_dtype& x)
{
	return matrix(A.ths.get()->Multiply(*A.ths.get(), x));
}

matrix matrix::Multiply_BT(const matrix& A, const matrix& B)
{
	return matrix(A.ths.get()->Multiply_BT(*A.ths.get(), *B.ths.get()));
}

matrix matrix::Hadamard(const matrix& A, const matrix& B)
{
	return matrix(A.ths.get()->Dotprod(*A.ths.get(), *B.ths.get()));
}

matrix_dtype matrix::Sum(const matrix& A)
{
	return A.ths.get()->Sum(*A.ths.get());
}

matrix matrix::Transpose(const matrix& A)
{
	return matrix(A.ths.get()->Transpose(*A.ths.get()));
}

void matrix::Add_inp(matrix& A, const matrix& B)
{
	A.ths.get()->Add_inp(*A.ths.get(), *B.ths.get());
}

void matrix::Add_inp(matrix& A, const matrix_dtype& x)
{
	A.ths.get()->Add_inp(*A.ths.get(), x);
}

void matrix::Multiply_inp(matrix& A, const matrix_dtype& x)
{
	A.ths.get()->Multiply_inp(*A.ths.get(), x);
}

void matrix::Subtract_inp(matrix& A, const matrix& B)
{
	A.ths.get()->Subtract_inp(*A.ths.get(), *B.ths.get());
}

void matrix::Hadamard_inp(matrix& A, const matrix& B)
{
	A.ths.get()->Dotprod_inp(*A.ths.get(), *B.ths.get());
}

void matrix::Pow_inp(matrix& A, int power)
{
	A.ths.get()->Pow_inp(*A.ths.get(), power);
}

void matrix::Transpose_inp(matrix& A)
{
	A.ths.get()->Transpose_inp(*A.ths.get());
}

matrix matrix::Tanh(matrix& A)
{
	return matrix(A.ths.get()->Tanh(*A.ths.get()));
}

matrix matrix::Tanh_D(const matrix& A)
{
	return matrix(A.ths.get()->Tanh_D(*A.ths.get()));
}

matrix matrix::Sigmoid(matrix& A)
{
	return matrix(A.ths.get()->Sigmoid(*A.ths.get()));
}

matrix matrix::Sigmoid_D(const matrix& A)
{
	return matrix(A.ths.get()->Sigmoid_D(*A.ths.get()));
}
