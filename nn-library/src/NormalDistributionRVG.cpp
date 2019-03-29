#include "NormalDistributionRVG.h"
#include "../../matrix-library/include/matrix.h"
#include <ctime>


void NormalDistributionRVG::random(matrix& mat)
{
	gen.seed(time(nullptr));
	for (int i = 0; i < mat.rows(); i++)
		for (int j = 0; j < mat.cols(); j++)
			MX(i, j, mat) = dst(gen);
}
