#ifdef MAT_ARMADILLO
#include "matrix_arma.h"
#include "matrix_default.h"


matrix_proxy matrix_arma::getPrx(int i, int j) const
{
	return std::move(matrix_proxy(this, i, j));
}

matrix_iface::ACCEL matrix_arma::actype() const
{
	return matrix_iface::ARMA_CPU;
}

void matrix_arma::create(int r, int c, matrix_dtype defaultValues)
{
	std::shared_ptr<arma::mat> p(new arma::mat(r, c));
	p->fill(defaultValues);
	matrix = std::move(p);
	rows_ = r;
	cols_ = c;
}

matrix_dtype* matrix_arma::getArr() const
{
	return (matrix_dtype*)(*matrix).memptr();
}

matrix_dtype matrix_arma::getAt(int i, int j) const
{
	return (*matrix)(i, j);
}

void matrix_arma::setAt(int i, int j, matrix_dtype& x) const
{
	(*matrix)(i, j) = x;
}

void matrix_arma::memcpy(matrix_iface& dst) const
{
	if (dst.actype() == ARMA_CPU)
	{
		matrix_arma* d = (matrix_arma*)&dst;
		*d->matrix = *matrix;
	}else if (dst.actype() == DEFAULT) {
		matrix_default* d = (matrix_default*)&dst;
		std::memcpy((double*)d->getArr(), (*matrix).memptr(), sizeof(double)*rows_*cols_);
	}
}

void matrix_arma::move(matrix_iface& dst)
{
	if(dst.actype() == ARMA_CPU)
	{
		matrix_arma* d = (matrix_arma*)&dst;
		d->matrix = matrix;
	}else if(dst.actype() == DEFAULT)
	{
		memcpy(dst);
	}
	matrix.reset();
	rows_ = 0; cols_ = 0;
}

bool matrix_arma::isEmpty() const
{
	return rows_ == 0 || cols_ == 0;
}

matrix_iface* matrix_arma::shallow_copy(const matrix_iface& x)
{
	if (x.isEmpty())
		return nullptr;
	if(x.actype() == ARMA_CPU)
	{
		matrix_arma *xx = (matrix_arma*)&x;
		matrix_arma *m = new matrix_arma;
		m->matrix = xx->matrix;
		m->rows_ = xx->rows_;
		m->cols_ = xx->cols_;
		return m;
	}
	return nullptr;
}

matrix_iface* matrix_arma::deep_copy(const matrix_iface& x)
{
	if (x.isEmpty())
		return nullptr;
	if (x.actype() == ARMA_CPU)
	{
		matrix_arma *xx = (matrix_arma*)&x;
		matrix_arma *m = new matrix_arma;
		*m->matrix = *xx->matrix;
		m->rows_ = xx->rows_;
		m->cols_ = xx->cols_;
		return m;
	}
	return nullptr;
}

void matrix_arma::cpy_ctor(const matrix_iface& cp)
{
	if (&cp == this)
		return;
	cols_ = cp.cols();
	rows_ = cp.rows();

	if (rows_ > 0 && cols_ > 0)
	{
		std::shared_ptr<arma::mat> ptr_a(new arma::mat(rows_,cols_));
		ptr_a->fill(0);
		matrix = std::move(ptr_a);
		cp.memcpy(*this);
	}
	else
	{
		matrix.reset();
	}
}

void matrix_arma::move_ctor(matrix_iface&& mv)
{
	if (&mv == this)
		return;
	cols_ = mv.cols();
	rows_ = mv.rows();
	mv.move(*this);
}

matrix_iface* matrix_arma::Add(const matrix_iface& A, const matrix_iface& B)
{
	if (A.rows() != B.rows() || A.cols() != B.cols()) {
		return new matrix_arma;
	}
	if (B.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		matrix_arma* b = (matrix_arma*)&B;
		arma::mat s = *a->matrix + *b->matrix;
		return new matrix_arma(s);
	}else if(B.actype() == DEFAULT)
	{
		arma::mat b((double*)B.getArr(),B.rows(),B.cols(), false);
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat s = *a->matrix + b;
		return new matrix_arma(s);
	}
}

matrix_iface* matrix_arma::Add(const matrix_iface& A, matrix_dtype x)
{
	matrix_arma* a = (matrix_arma*)&A;
	arma::mat s = *a->matrix + x;
	return new matrix_arma(s);
}

matrix_iface* matrix_arma::Subtract(const matrix_iface& A, const matrix_iface& B)
{
	if (A.rows() != B.rows() || A.cols() != B.cols()) {
		return new matrix_arma;
	}
	if (B.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		matrix_arma* b = (matrix_arma*)&B;
		arma::mat s = *a->matrix - *b->matrix;
		return new matrix_arma(s);
	}
	else if (B.actype() == DEFAULT)
	{
		arma::mat b((double*)B.getArr(), B.rows(), B.cols(), false);
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat s = *a->matrix - b;
		return new matrix_arma(s);
	}
}

matrix_iface* matrix_arma::Multiply(const matrix_iface& A, const matrix_iface& B)
{
	if (A.cols() != B.rows()) {
		return new matrix_arma;
	}
	if (B.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		matrix_arma* b = (matrix_arma*)&B;
		arma::mat s = *a->matrix * *b->matrix;
		return new matrix_arma(s);
	}
	else if (B.actype() == DEFAULT)
	{
		arma::mat b((double*)B.getArr(), B.rows(), B.cols(), false);
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat s = *a->matrix * b;
		return new matrix_arma(s);
	}
}

matrix_iface* matrix_arma::Multiply(const matrix_iface& A, matrix_dtype x)
{
	if(A.actype() == ARMA_CPU)
	{
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat s = *a->matrix * x;
		return new matrix_arma(s);
	}
}

matrix_iface* matrix_arma::Multiply_BT(const matrix_iface& A, const matrix_iface& B)
{
	if (A.cols() != B.cols()) {
		return new matrix_arma;
	}
	if (B.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		matrix_arma* b = (matrix_arma*)&B;
		arma::mat s = *a->matrix * (*b->matrix).t();
		return new matrix_arma(s);
	}
	else if (B.actype() == DEFAULT)
	{
		arma::mat b((double*)B.getArr(), B.rows(), B.cols(), false);
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat s = *a->matrix * b.t();
		return new matrix_arma(s);
	}
}

matrix_iface* matrix_arma::Dotprod(const matrix_iface& A, const matrix_iface& B)
{
	if (A.rows() != B.rows() || A.cols() != B.cols()) {
		return new matrix_arma;
	}
	if (B.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		matrix_arma* b = (matrix_arma*)&B;
		arma::mat s = *a->matrix % (*b->matrix);
		return new matrix_arma(s);
	}
	else if (B.actype() == DEFAULT)
	{
		arma::mat b((double*)B.getArr(), B.rows(), B.cols(), false);
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat s = *a->matrix % b;
		return new matrix_arma(s);
	}
}

matrix_dtype matrix_arma::Sum(const matrix_iface& A)
{
	if(A.actype() == ARMA_CPU)
	{
		matrix_arma* a = (matrix_arma*)&A;
		matrix_dtype x = arma::accu(*a->matrix);
		return x;
	}
}

matrix_iface* matrix_arma::Transpose(const matrix_iface& A)
{
	if (A.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat s = (*a->matrix).t();
		return new matrix_arma(s);
	}
}

void matrix_arma::Add_inp(matrix_iface& A, const matrix_iface& B)
{
	if (A.rows() != B.rows() || A.cols() != B.cols()) {
		return;
	}
	if (B.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		matrix_arma* b = (matrix_arma*)&B;
		*a->matrix += *b->matrix;
	}
	else if (B.actype() == DEFAULT)
	{
		arma::mat b((double*)B.getArr(), B.rows(), B.cols(), false);
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat s = *a->matrix += b;
	}
}

void matrix_arma::Add_inp(matrix_iface& A, matrix_dtype x)
{
	if (A.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		*a->matrix += x;
	}
}

void matrix_arma::Multiply_inp(matrix_iface& A, matrix_dtype x)
{
	if (A.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		*a->matrix *= x;
	}
}

void matrix_arma::Subtract_inp(matrix_iface& A, const matrix_iface& B)
{
	if (A.rows() != B.rows() || A.cols() != B.cols()) {
		return;
	}
	if (B.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		matrix_arma* b = (matrix_arma*)&B;
		*a->matrix -= *b->matrix;
	}
	else if (B.actype() == DEFAULT)
	{
		arma::mat b((double*)B.getArr(), B.rows(), B.cols(), false);
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat s = *a->matrix -= b;
	}
}

void matrix_arma::Dotprod_inp(matrix_iface& A, const matrix_iface& B)
{
	if (A.rows() != B.rows() || A.cols() != B.cols()) {
		return;
	}
	if (B.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		matrix_arma* b = (matrix_arma*)&B;
		*a->matrix %= *b->matrix;
	}
	else if (B.actype() == DEFAULT)
	{
		arma::mat b((double*)B.getArr(), B.rows(), B.cols(), false);
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat s = *a->matrix %= b;
	}
}

void matrix_arma::Pow_inp(matrix_iface& A, int power)
{
	if (A.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		*a->matrix = arma::pow(*a->matrix, power);
	}
}

void matrix_arma::Transpose_inp(matrix_iface& A)
{
	if (A.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		*a->matrix = (*a->matrix).t();
	}
}

matrix_iface* matrix_arma::Tanh(const matrix_iface& A)
{
	if (A.actype() == ARMA_CPU) {
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat s = arma::tanh(*a->matrix);
		return new matrix_arma(s);
	}
}

matrix_iface* matrix_arma::Tanh_D(const matrix_iface& A)
{
	if (A.actype() == ARMA_CPU)
	{
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat r = 1 - arma::pow(arma::tanh(*a->matrix), 2);

		return new matrix_arma(r);
	}
}

matrix_iface* matrix_arma::Sigmoid(const matrix_iface& A)
{
	if (A.actype() == ARMA_CPU)
	{
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat r = 1 / (1 + arma::exp(-(*a->matrix)));

		return new matrix_arma(r);
	}
}

matrix_iface* matrix_arma::Sigmoid_D(const matrix_iface& A)
{
	if (A.actype() == ARMA_CPU)
	{
		matrix_arma* a = (matrix_arma*)&A;
		arma::mat y =  1 / (1 + arma::exp(-(*a->matrix)));
		arma::mat r = y % (1 - y);
		return new matrix_arma(r);
	}
}

matrix_arma::matrix_arma()
{
}

matrix_arma::matrix_arma(arma::mat& m)
{
	std::shared_ptr<arma::mat> p(new arma::mat);
	*p = m;
	matrix = std::move(p);
	rows_ = m.n_rows;
	cols_ = m.n_cols;
}


matrix_arma::~matrix_arma()
{
}
#endif
