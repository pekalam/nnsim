#ifdef MAT_ARRAYFIRE
#include "matrix_af.h"
#include "matrix_proxy.h"
#include "matrix_default.h"

matrix_proxy matrix_af::getPrx(int i, int j) const
{
	return std::move(matrix_proxy(this, i, j));
}

matrix_dtype matrix_af::getAt(int i, int j) const
{
	double d = (*arr.get())(i, j).scalar<data_t>();
	return std::move(d);
}

void matrix_af::setAt(int i, int j, matrix_dtype& x) const
{
	(*arr.get())(i, j) = x;
}

matrix_iface* matrix_af::Tanh(const matrix_iface& A)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A; 
		af::array r = af::tanh(*a->arr.get());
		
		return new matrix_af(r);
	}
}

matrix_iface* matrix_af::Tanh_D(const matrix_iface& A)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		af::array r = 1 - pow(af::tanh(*a->arr.get()), 2);
		
		return new matrix_af(r);
	}
}

matrix_iface* matrix_af::Sigmoid(const matrix_iface& A)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		af::array r = af::sigmoid(*a->arr.get());
		
		return new matrix_af(r);
	}
}

matrix_iface* matrix_af::Sigmoid_D(const matrix_iface& A)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		af::array r = af::sigmoid(*a->arr.get())*(1 - af::sigmoid(*a->arr.get()));
		
		return new matrix_af(r);
	}
}

matrix_af::matrix_af(int r, int c, matrix_dtype def)
{
	matrix_af::create(r, c, def);
}

matrix_af::matrix_af()
{
}

matrix_af::matrix_af(af::array& a)
{
	std::shared_ptr<af::array> p(new af::array());
	*p.get() = a;
	arr = std::move(p);
	rows_ = a.dims()[0];
	cols_ = a.dims()[1];
}

void matrix_af::create(int r, int c, matrix_dtype defaultValues)
{
	rows_ = r;
	cols_ = c;
	if (rows_ > 0 && cols_ > 0)
	{
		dim_t d[2] = {rows_, cols_};
		af_array handle;
		af_constant(&handle, defaultValues, 2, d, f64);
		std::shared_ptr<af::array> ptr_a(new af::array(handle));
		arr = std::move(ptr_a);
	}
}

matrix_dtype* matrix_af::getArr() const
{
	//arr->host(hostPtr.get()); 
	return nullptr;
}

void matrix_af::memcpy(matrix_iface& dst) const
{
	if (dst.actype() == AF_GPU) {
		matrix_af* a = (matrix_af*)&dst;
		*a->arr.get() = *arr.get();
	}else if(dst.actype() == DEFAULT)
	{
		matrix_default* d = (matrix_default*)&dst;
		double* hst = arr->host<double>();
		std::shared_ptr<matrix_dtype> dp(new matrix_dtype[rows_*cols_]);
		for (int i = 0; i < rows_*cols_; i++)
			dp.get()[i] = hst[i];
		d->_matrix = std::move(dp);
		af::freeHost(hst);
	}
}

void matrix_af::fromArray(matrix_dtype* arr, int r, int c)
{
	std::shared_ptr<af::array> ptr_a(new af::array(r,c, (double*)arr));
	this->arr = std::move(ptr_a);
}

void matrix_af::move(matrix_iface& dst)
{
	
	if (dst.actype() == AF_GPU) {
		matrix_af* a = (matrix_af*)&dst;
		a->arr = arr;
	}
	else if (dst.actype() == DEFAULT)
	{
		memcpy(dst);
	}
	arr.reset();
	rows_ = cols_ = 0;
}

matrix_af::~matrix_af()
{
}

bool matrix_af::isEmpty() const
{
	return rows_ == 0 || cols_ == 0;
}

matrix_iface* matrix_af::shallow_copy(const matrix_iface& x)
{
	return nullptr;
}

matrix_iface* matrix_af::deep_copy(const matrix_iface& x)
{
	return nullptr;
}

void matrix_af::cpy_ctor(const matrix_iface& cp)
{
	if (&cp == this)
		return;
	cols_ = cp.cols_;
	rows_ = cp.rows_;

	if (rows_ > 0 && cols_ > 0)
	{
		std::shared_ptr<af::array> ptr_a(new af::array());
		arr = std::move(ptr_a);
		cp.memcpy(*this);
	}else
	{
		arr.reset();
	}
}

void matrix_af::move_ctor(matrix_iface&& mv)
{
	if (&mv == this)
		return;
	cols_ = mv.cols_;
	rows_ = mv.rows_;
	mv.move(*this);
}

matrix_iface* matrix_af::Add(const matrix_iface& A, const matrix_iface& B)
{
	if (A.actype() == AF_GPU && B.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_af* b = (matrix_af*)&B;
		af::array r = *(a->arr.get()) + *(b->arr.get());
		
		return new matrix_af(r);
	}else if (A.actype() == AF_GPU && B.actype() == DEFAULT)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_default* b = (matrix_default*)&B;
		af::array barr(B.rows(), B.cols(), (double*)b->_matrix.get());
		af::array r = *(a->arr.get()) + barr;
		
		return new matrix_af(r);
	}
}

matrix_iface* matrix_af::Add(const matrix_iface& A, matrix_dtype x)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;

		af::array r = *(a->arr.get()) + x;
		
		return new matrix_af(r);
	}
}

matrix_iface* matrix_af::Subtract(const matrix_iface& A, const matrix_iface& B)
{
	if (A.actype() == AF_GPU && B.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_af* b = (matrix_af*)&B;
		af::array r = *(a->arr.get()) - *(b->arr.get());
		
		return new matrix_af(r);
	}
	else if (A.actype() == AF_GPU && B.actype() == DEFAULT)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_default* b = (matrix_default*)&B;
		af::array barr(B.rows(), B.cols(), (double*)b->_matrix.get());
		af::array r = *(a->arr.get()) - barr;
		
		return new matrix_af(r);
	}
}

matrix_iface* matrix_af::Multiply(const matrix_iface& A, const matrix_iface& B)
{
	if (A.actype() == AF_GPU && B.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_af* b = (matrix_af*)&B;
		af::array r = af::matmul(*(a->arr.get()), *(b->arr.get()));
		
		return new matrix_af(r);
	}
	else if (A.actype() == AF_GPU && B.actype() == DEFAULT)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_default* b = (matrix_default*)&B;
		af::array barr(B.rows(), B.cols(), (double*)b->_matrix.get());
		af::array r = af::matmul(*(a->arr.get()) , barr);
		
		return new matrix_af(r);
	}
}

matrix_iface* matrix_af::Multiply(const matrix_iface& A, const matrix_dtype x)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;

		af::array r = (*a->arr.get()) * x;
		
		return new matrix_af(r);
	}
}

matrix_iface* matrix_af::Multiply_BT(const matrix_iface& A, const matrix_iface& B)
{
	if (A.actype() == AF_GPU && B.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_af* b = (matrix_af*)&B;
		af::array r = af::matmul(*(a->arr.get()), *(b->arr.get()), AF_MAT_NONE, AF_MAT_TRANS);
		
		return new matrix_af(r);
	}
	else if (A.actype() == AF_GPU && B.actype() == DEFAULT)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_default* b = (matrix_default*)&B;
		af::array barr(B.rows(), B.cols(), (double*)b->_matrix.get());
		af::array r = af::matmul(*(a->arr.get()), barr, AF_MAT_NONE, AF_MAT_TRANS);
		
		return new matrix_af(r);
	}
}

matrix_iface* matrix_af::Dotprod(const matrix_iface& A, const matrix_iface& B)
{
	if (A.actype() == AF_GPU && B.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_af* b = (matrix_af*)&B;
		af::array r = *(a->arr.get()) * *(b->arr.get());
		
		return new matrix_af(r);
	}
	else if (A.actype() == AF_GPU && B.actype() == DEFAULT)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_default* b = (matrix_default*)&B;
		af::array barr(B.rows(), B.cols(), (double*)b->_matrix.get());
		af::array r = *(a->arr.get()) * barr;
		
		return new matrix_af(r);
	}
}

matrix_dtype matrix_af::Sum(const matrix_iface& A)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		double r = af::sum<double>(*(a->arr.get()));
		return r;
	}
}

matrix_iface* matrix_af::Transpose(const matrix_iface& A)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;

		af::array r = af::transpose(*(a->arr.get()));
		
		return new matrix_af(r);
	}
}

void matrix_af::Add_inp(matrix_iface& A, const matrix_iface& B)
{
	if (A.actype() == AF_GPU && B.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_af* b = (matrix_af*)&B;
		(*a->arr.get()) += *b->arr.get();
		af::eval((*a->arr.get()));
	}
	else if (A.actype() == AF_GPU && B.actype() == DEFAULT)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_default* b = (matrix_default*)&B;
		af::array barr(B.rows(), B.cols(), (double*)b->_matrix.get());
		*(a->arr.get()) += barr;
		af::eval((*a->arr.get()));
	}
}

void matrix_af::Add_inp(matrix_iface& A, const matrix_dtype x)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		(*a->arr.get()) += x;
		af::eval((*a->arr.get()));
	}
}

void matrix_af::Multiply_inp(matrix_iface& A, const matrix_dtype x)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		(*a->arr.get()) *= x;
		af::eval((*a->arr.get()));
	}
}

void matrix_af::Subtract_inp(matrix_iface& A, const matrix_iface& B)
{
	if (A.actype() == AF_GPU && B.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_af* b = (matrix_af*)&B;
		(*a->arr.get()) -= *b->arr.get();
		af::eval((*a->arr.get()));
	}
	else if (A.actype() == AF_GPU && B.actype() == DEFAULT)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_default* b = (matrix_default*)&B;
		af::array barr(B.rows(), B.cols(), (double*)b->_matrix.get());
		*(a->arr.get()) -= barr;
		af::eval((*a->arr.get()));
	}
}

void matrix_af::Dotprod_inp(matrix_iface& A, const matrix_iface& B)
{
	if (A.actype() == AF_GPU && B.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_af* b = (matrix_af*)&B;
		*a->arr.get() = *a->arr.get() * *b->arr.get();
		af::eval((*a->arr.get()));
	}
	else if (A.actype() == AF_GPU && B.actype() == DEFAULT)
	{
		matrix_af* a = (matrix_af*)&A;
		matrix_default* b = (matrix_default*)&B;
		af::array barr(B.rows(), B.cols(), (double*)b->_matrix.get());
		*a->arr.get() = *a->arr.get() * barr;
		af::eval((*a->arr.get()));
	}
}

void matrix_af::Pow_inp(matrix_iface& A, int power)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		*a->arr.get() = af::pow(*a->arr.get(), power);
		af::eval((*a->arr.get()));
	}
}

void matrix_af::Transpose_inp(matrix_iface& A)
{
	if (A.actype() == AF_GPU)
	{
		matrix_af* a = (matrix_af*)&A;
		af::transposeInPlace(*a->arr.get());
		af::eval((*a->arr.get()));
		a->rows_ = a->arr->dims()[0];
		a->cols_ = a->arr->dims()[1];
	}
}
#endif