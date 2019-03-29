#pragma once

#include "matrix.h"

inline double arithmectic_mean(matrix const &x)
{
	double mean = 0;
	double total = x.rows()*x.cols();
	for (int i = 0; i < x.rows(); i++)
		for (int j = 0; j < x.cols(); j++)
			mean += MX(i, j, x) / total;
	return mean;
}

inline matrix standard_deviation(matrix const &x)
{
	double ar_mean = arithmectic_mean(x);
	matrix m(x.rows(), x.cols());
	for (int i = 0; i < x.rows(); i++)
		for (int j = 0; j < x.cols(); j++)
			MX(i, j, m) = std::sqrt(std::pow(MX(i, j, m) - ar_mean, 2));
	return m;
}


