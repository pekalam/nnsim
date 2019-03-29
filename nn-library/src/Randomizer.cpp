
#include "Randomizer.h"
#include "../../matrix-library/include/matrix.h"
#include <time.h>

std::default_random_engine Randomizer::gen;

void Randomizer::fromRange(double lowerBound, double upperBound, matrix& x)
{
	gen.seed(time(NULL));
	std::uniform_real_distribution<double> dst(lowerBound, upperBound);
	for (int i = 0; i < x.rows(); i++)
		for (int j = 0; j < x.cols(); j++)
			MX(i, j, x) = dst(gen);
}

void Randomizer::fromNormalDist(matrix & x)
{
	gen.seed(time(NULL));
	std::normal_distribution<double> dst;
	for (int i = 0; i < x.rows(); i++)
		for (int j = 0; j < x.cols(); j++)
			MX(i, j, x) = dst(gen);
}
