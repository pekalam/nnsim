#include "RandomValueGeneratorFactory.h"
#include "RangeRVG.h"
#include "NormalDistributionRVG.h"

std::unique_ptr<RandomValueGeneratorFactory> RandomValueGeneratorFactory::instance_;

void RandomValueGeneratorFactory::initTypeMap()
{
	registerActivationFunction("01", std::make_unique<RangeRVG>(0, 1).get());
	registerActivationFunction("N01", std::make_unique<NormalDistributionRVG>().get());
}
