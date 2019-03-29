
#include "ReLU.h"
#include "../../matrix-library/include/matrix.h"



double ReLU::Call(double x) const
{
	return x >= 0 ? x : 0;
}

void ReLU::Call(matrix& x) const
{ 
	for(int i = 0; i < x.rows(); i++)
		for(int j = 0; j < x.cols(); j++)
		{
			if (MX(i, j, x) < 0.0)
				MX(i, j, x) = 0;
		}
}

double ReLU::DerivativeCall(double x) const
{
	return x >= 0 ? 1 : 0;
}

matrix ReLU::DerivativeCall(const matrix& x) const
{
	matrix r(x.rows(), x.cols(), 0);
	for (int i = 0; i < x.rows(); i++)
		for (int j = 0; j < x.cols(); j++)
		{
			if (MX(i, j, x) >= 0.0)
				MX(i, j, r) = MX(i, j, x);
		}
	return r;
}
