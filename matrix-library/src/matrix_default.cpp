#include "matrix_default.h"
#include "matrix_proxy.h"
#include "matrix_af.h"
#include "matrix.h"


matrix_iface* matrix_default::Tanh(const matrix_iface& A)
{
	matrix_default *r = new matrix_default(A.rows(), A.cols());
	for (int i = 0; i < A.rows(); i++)
		for (int j = 0; j < A.cols(); j++)
			MX(i, j, *r) = (2 / (1 + exp(-2 * MX(i, j, A)))) - 1;
	return r;
}

matrix_iface* matrix_default::Tanh_D(const matrix_iface& A)
{
	matrix_default *r = new matrix_default(A.rows(), A.cols());
	for (int i = 0; i < A.rows(); i++)
		for (int j = 0; j < A.cols(); j++)
			MX(i, j, *r) = 1 - pow(((2 / (1 + exp(-2 * MX(i, j, A)))) - 1), 2);
	return r;
}

matrix_iface* matrix_default::Sigmoid(const matrix_iface& A)
{
	matrix_default *r = new matrix_default(A.rows(), A.cols());
	for (int i = 0; i < A.rows(); i++)
		for (int j = 0; j < A.cols(); j++)
			MX(i, j, *r) = 1 / (1 + exp(-MX(i, j, A)));
	return r;
}

matrix_iface* matrix_default::Sigmoid_D(const matrix_iface& A)
{
	matrix_default *r = new matrix_default(A.rows(), A.cols());
	for (int i = 0; i < A.rows(); i++)
		for (int j = 0; j < A.cols(); j++)
			MX(i, j, *r) = (1 / (1 + exp(-MX(i, j, A))))*(1 - (1 / (1 + exp(-MX(i, j, A)))));
	return r;
}

matrix_proxy matrix_default::getPrx(int i, int j) const
{
	return matrix_proxy(this, i, j);
}

matrix_dtype matrix_default::getAt(int i, int j) const
{
	return _matrix.get()[j*rows_ + i];
}

void matrix_default::setAt(int i, int j, matrix_dtype& x) const
{
	_matrix.get()[j*rows_ + i] = x;
}

matrix_default::matrix_default(int r, int c, matrix_dtype def)
{
	matrix_default::create(r, c, def);
}

void matrix_default::create(int r, int c, matrix_dtype defaultValues)
{
	rows_ = r;
	cols_ = c;
	if (c >= 0 && r >= 0)
	{
		std::shared_ptr<matrix_dtype> p(new matrix_dtype[r * c]);
		_matrix = std::move(p);
		for (int i = 0; i < r*c; i++)
			_matrix.get()[i] = defaultValues;
	}
}

void matrix_default::memcpy(matrix_iface& dst) const
{
	if(dst.actype() == DEFAULT)
		std::memcpy(dst.getArr(), _matrix.get(), sizeof(matrix_dtype)*rows_*cols_);
#ifdef MAT_ARRAYFIRE
	if(dst.actype() == AF_GPU)
	{
		matrix_af* d = (matrix_af*)&dst;
		d->fromArray(_matrix.get(), rows_, cols_);
	}
#endif
#ifdef MAT_ARMADILLO
	if(dst.actype() == ARMA_CPU)
	{
		matrix_dtype* dstArr = dst.getArr();
		double* dstArrD = (double*)dstArr;
		std::memcpy((double*)dst.getArr(), (double*)_matrix.get(), sizeof(double)*rows_*cols_);
	}
#endif
}

matrix_dtype* matrix_default::getArr() const
{
	return _matrix.get();
}

bool matrix_default::isEmpty() const
{
	return (_matrix == nullptr) || (_matrix.get() == nullptr) || (rows_ == 0) || (cols_ == 0);
}

void matrix_default::cpy_ctor(const matrix_iface& cp)
{
	if (&cp == this)
		return;
	cols_ = cp.cols();
	rows_ = cp.rows();
	std::shared_ptr<matrix_dtype> p(new matrix_dtype[rows_ * cols_]);
	_matrix = std::move(p);
	cp.memcpy(*this);
}

void matrix_default::move_ctor(matrix_iface&& mv)
{
	if (&mv == this)
		return;
	cols_ = mv.cols();
	rows_ = mv.rows();
	mv.move(*this);
}

void matrix_default::move(matrix_iface& dst)
{
	if(dst.actype() == DEFAULT)
	{
		matrix_default* d = (matrix_default*)&dst;
		d->_matrix = _matrix;
	}
#ifdef MAT_ARRAYFIRE
	if(dst.actype() == AF_GPU)
	{
		matrix_af* d = (matrix_af*)&dst;
		d->fromArray(_matrix.get(), rows_, cols_);
	}
#endif
	_matrix.reset();
}

matrix_iface* matrix_default::shallow_copy(const matrix_iface& x)
{
	const matrix_default* xx = (matrix_default*)&x;
	matrix_default *r = new matrix_default(xx->rows_, xx->cols_);
	r->_matrix = xx->_matrix;
	r->rows_ = xx->rows_;
	r->cols_ = xx->cols_;
	return r;
}

matrix_iface* matrix_default::deep_copy(const matrix_iface& x)
{
	matrix_default* r = new matrix_default(x.rows(), x.cols());
	if (!x.isEmpty())
	{
		std::shared_ptr<matrix_dtype> p(new matrix_dtype[r->rows_ * r->cols_]);
		r->_matrix = std::move(p);
		x.memcpy(*r);
	}
	return r;
}

matrix_iface* matrix_default::Add(const matrix_iface&A, const matrix_iface&B) {
	if (A.rows() != B.rows() || A.cols() != B.cols()) {
		return new matrix_default(0,0);
	}
	
	matrix_default *r = new matrix_default(A.rows(), A.cols());
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < B.cols(); ++j)
			MX(i, j, *r) = MX(i, j, A) + MX(i, j, B);
	return r;
}

void matrix_default::Add_inp(matrix_iface& A, const matrix_iface& B) {
	if (A.rows() != B.rows() || A.cols() != B.cols())
		return;
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < B.cols(); ++j)
			MX(i, j, A) = MX(i, j, A) + MX(i, j, B);
}

void matrix_default::Add_inp(matrix_iface& A, matrix_dtype x) {
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < A.cols(); ++j)
			MX(i, j, A) = MX(i, j, A) + x;
}

matrix_iface* matrix_default::Subtract(const matrix_iface&A, const matrix_iface&B) {
	if (A.rows() != B.rows() || A.cols() != B.cols()) {
		return new matrix_default(0,0);
	}
	matrix_default *r = new matrix_default(A.rows(), A.cols());
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < B.cols(); ++j)
			MX(i, j, *r) = MX(i, j, A) - MX(i, j, B);
	return r;
}


void matrix_default::Subtract_inp(matrix_iface& A, const matrix_iface& B) {
	if (A.rows() != B.rows() || A.cols() != B.cols()) {
		return;
	}
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < B.cols(); ++j)
			MX(i, j, A) = MX(i, j, A) - MX(i, j, B);
}

matrix_iface* matrix_default::Multiply(const matrix_iface &A, const matrix_iface &B) {
	if (A.cols() != B.rows()) {
		return new matrix_default(0,0);
	}
	matrix_default *r = new matrix_default(A.rows(), B.cols());
	for (int ia = 0; ia < A.rows(); ia++)
		for (int jb = 0; jb < B.cols(); jb++)
			for (int j = 0; j < A.cols(); ++j) {
				matrix_dtype x = MX(ia, j, A) * MX(j, jb, B);
				MX(ia, jb, *r) += x;
			}
	return r;
}

matrix_iface* matrix_default::Multiply(const matrix_iface &A, matrix_dtype x) {
	matrix_default *r = new matrix_default(A.rows(), A.cols());
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < A.cols(); ++j)
			MX(i, j, *r) = MX(i, j, A) * x;
	return r;
}


void matrix_default::Multiply_inp(matrix_iface& A, matrix_dtype x) {
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < A.cols(); ++j)
			MX(i, j, A) = MX(i, j, A) * x;
}

matrix_iface* matrix_default::Multiply_BT(const matrix_iface &A, const matrix_iface &B) {
	if (A.cols() != B.cols()) {
		return new matrix_default(0,0);
	}
	matrix_default *r = new matrix_default(A.rows(), B.rows());
	for (int ia = 0; ia < A.rows(); ia++)
		for (int jb = 0; jb < B.rows(); jb++)
			for (int j = 0; j < A.cols(); ++j) {
				matrix_dtype x = MX(ia, j, A) * MX_T(j, jb, B);
				MX(ia, jb, *r) += x;
			}
	return r;
}


matrix_iface* matrix_default::Dotprod(const matrix_iface &A, const matrix_iface &B) {
	if (A.rows() != B.rows() || A.cols() != B.cols()) {
		return new matrix_default(0,0);
	}
	matrix_default *r = new matrix_default(A.rows(), B.cols());
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < B.cols(); ++j)
			MX(i, j, *r) = MX(i, j, A) * MX(i, j, B);
	return r;
}


void matrix_default::Dotprod_inp(matrix_iface& A, const matrix_iface& B) {
	if (A.rows() != B.rows() || A.cols() != B.cols()) {
		return;
	}
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < B.cols(); ++j)
			MX(i, j, A) = MX(i, j, A) * MX(i, j, B);
}

void matrix_default::Pow_inp(matrix_iface& A, int power)
{
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < A.cols(); ++j)
			MX(i, j, A) = pow((matrix_dtype)MX(i, j, A), power);
}

matrix_dtype matrix_default::Sum(const matrix_iface &A) {
	matrix_dtype s = 0;
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < A.cols(); ++j)
			s += MX(i, j, A);
	return s;
}

matrix_iface* matrix_default::Transpose(const matrix_iface &A) {
	matrix_default *r = new matrix_default(A.cols(), A.rows());
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < A.cols(); ++j)
			MX(j, i, *r) = MX(i, j, A);
	return r;
}

void matrix_default::Transpose_inp(matrix_iface &A) {
	int size = A.rows() * A.cols() - 1;
	int t; // holds element to be replaced, eventually becomes next element to move 
	int next; // location of 't' to be moved 
	int cycleBegin; // holds start of cycle 
	int i; // iterator 

	i = 1; // Note that A[0] and A[size-1] won't move 
	while (i < size)
	{
		cycleBegin = i;
		t = i;
		do
		{
			next = (i*A.rows()) % size;
			double tmp = A.getArr()[next];
			A.getArr()[next] = A.getArr()[t];
			A.getArr()[t] = tmp;
			i = next;
		} while (i != cycleBegin);
	}
}

matrix_iface* matrix_default::Add(const matrix_iface &A, const matrix_dtype x) {
	matrix_default *r = new matrix_default(A.rows(), A.cols());
	for (int i = 0; i < A.rows(); ++i)
		for (int j = 0; j < A.cols(); ++j)
			MX(i, j, *r) = MX(i, j, A) + x;
	return r;
}