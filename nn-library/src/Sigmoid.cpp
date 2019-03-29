
#include "Sigmoid.h"
#include "../../matrix-library/include/matrix.h"



double Sigmoid::Call(double x) const
{
	return 1 / (1+exp(-x));
}

void Sigmoid::Call(matrix& x) const
{
	x = matrix::Sigmoid(x);
}

double Sigmoid::DerivativeCall(double x) const
{
	return (1 / (1 + exp(-x)))*(1 - (1 / (1 + exp(-x))));
}

matrix Sigmoid::DerivativeCall(const matrix& x) const
{
	return matrix::Sigmoid_D(x);
}

