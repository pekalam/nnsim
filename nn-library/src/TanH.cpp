
#include "TanH.h"
#include "../../matrix-library/include/matrix.h"


matrix TanH::DerivativeCall(const matrix& x) const
{
	return matrix::Tanh_D(x);
}


double TanH::Call(double x) const
{
	return (2 / (1 + exp(-2 * x))) - 1;
}

void TanH::Call(matrix& x) const
{
	x = matrix::Tanh(x);
}

double TanH::DerivativeCall(double x) const
{
	return 1 - pow((2 / (1 + exp(-2 * x))) - 1, 2);
}
