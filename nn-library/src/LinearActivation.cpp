
#include "LinearActivation.h"
#include "../../matrix-library/include/matrix.h"


double LinearActivation::Call(double x) const
{
	return x;
}

void LinearActivation::Call(matrix& x) const
{
	return;
}

double LinearActivation::DerivativeCall(double x) const
{
	return 1;
}

matrix LinearActivation::DerivativeCall(const matrix& x) const
{
	return matrix(x.rows(), x.cols(), 1);
}
