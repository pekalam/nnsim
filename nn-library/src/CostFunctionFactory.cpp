#include "CostFunctionFactory.h"
#include "QuadraticCost.h"

std::unique_ptr<CostFunctionFactory> CostFunctionFactory::instance_;

void CostFunctionFactory::initTypeMap()
{
	registerActivationFunction("Quadratic", std::make_unique<QuadraticCost>().get());
}
