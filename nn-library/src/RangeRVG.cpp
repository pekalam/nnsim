#include "RangeRVG.h"
#include <ctime>
#include "../../matrix-library/include/matrix.h"

void RangeRVG::random(matrix& mat)
{
	gen.seed(time(nullptr));
	for (int i = 0; i < mat.rows(); i++)
		for (int j = 0; j < mat.cols(); j++)
			MX(i, j, mat) = dst(gen);
}

RangeRVG::RangeRVG(double lowerBound, double upperBound) : lowerBound(lowerBound), upperBound(upperBound)
{
}

RangeRVG::~RangeRVG()
{
}
