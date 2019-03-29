
#include "FFNetworkLayer.h"
#include "ActivationFunction.h"
#include "LinearActivation.h"
#include "ReLU.h"
#include "Sigmoid.h"
#include "TanH.h"
#include "Randomizer.h"
#include "ActivationFunctionFactory.h"
#include "RandomValueGenerator.h"



FFNetworkLayer::FFNetworkLayer(int neuronsCount, const std::string& activationFunctionName, double bias) :
	activationFunction(ActivationFunctionFactory::instance().createActivationFunction(activationFunctionName)),
	activationFunctionName(activationFunctionName),
	neuronsCount(neuronsCount), 
	neuronInputs(0),
	layerNumber(-1),
	output(neuronsCount, 1),
	biases(neuronsCount, 1, bias),
	weights(0,0)
{
}

FFNetworkLayer::FFNetworkLayer(int neuronsCount, const std::string& activationFunctionName, int neuronSynapses,
	double bias) :
	activationFunction(ActivationFunctionFactory::instance().createActivationFunction(activationFunctionName)),
	activationFunctionName(activationFunctionName),
	neuronsCount(neuronsCount),
	neuronInputs(neuronSynapses),
	layerNumber(-1),
	output(neuronsCount, 1),
	biases(neuronsCount, 1, bias),
	weights(neuronsCount, neuronSynapses)
{
}

void FFNetworkLayer::setActivationFunction(const std::string& activationFunctionName)
{
	this->activationFunctionName = activationFunctionName;
	activationFunction = ActivationFunctionFactory::instance().createActivationFunction(activationFunctionName);
}

void FFNetworkLayer::setWeights(const matrix& weights)
{
	if(this->weights.rows() == weights.rows() && this->weights.cols() == weights.cols())
		this->weights = std::move(matrix::deep_copy(weights));
}

void FFNetworkLayer::setBiases(const matrix& biases)
{
	if (biases.rows() == neuronsCount && biases.cols() == 1)
		this->biases = std::move(matrix::deep_copy(biases));
}

void FFNetworkLayer::randomizeBiases(RandomValueGenerator& randomValueGenerator)
{
	randomValueGenerator.random(biases);
}

void FFNetworkLayer::randomizeWeights(RandomValueGenerator& randomValueGenerator)
{
	randomValueGenerator.random(weights);
}
