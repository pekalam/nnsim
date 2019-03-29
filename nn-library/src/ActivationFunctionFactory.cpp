#include "ActivationFunctionFactory.h"
#include "ActivationFunction.h"
#include "Sigmoid.h"
#include "TanH.h"
#include "LinearActivation.h"


std::unique_ptr<ActivationFunctionFactory> ActivationFunctionFactory::instance_;

void ActivationFunctionFactory::initTypeMap()
{
	registerActivationFunction("Sigmoid", std::make_unique<Sigmoid>().get());
	registerActivationFunction("TanH", std::make_unique<TanH>().get());
	registerActivationFunction("Linear", std::make_unique<LinearActivation>().get());
	registerActivationFunction("ReLU", std::make_unique<LinearActivation>().get());
}
