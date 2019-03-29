
#include "QuadraticCost.h"
#include "../../matrix-library/include/matrix.h"

double QuadraticCost::Call(const matrix &x, const matrix &t) {
	matrix er = matrix::Subtract(x, t);
	matrix sqer = matrix::Hadamard(er, er);
	return matrix::Sum(sqer)*(double)0.5;
}

double QuadraticCost::DerivativeCall(const matrix &x, const matrix &t) {
	matrix er = matrix::Subtract(x, t);
	return matrix::Sum(er);
}

matrix QuadraticCost::mCall(const matrix &x, const matrix &t) {
	matrix er = matrix::Subtract(x, t);
	matrix sqer = matrix::Hadamard(er, er);
	return matrix::Multiply(sqer, (double)0.5);
}

matrix QuadraticCost::mDerivativeCall(const matrix &x, const matrix &t) {
	matrix er = matrix::Subtract(x, t);
	return er;
}
