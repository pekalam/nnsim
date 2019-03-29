#pragma once

#include "../../matrix-library/include/matrix.h"
#include "ActivationFunction.h"
#include <string>
#include "FFNeuralNetwork.h"
#include "RandomValueGenerator.h"
#include "RandomValueGeneratorFactory.h"

class FFNetworkLayer
{
	friend class FFNeuralNetwork;
public:
	FFNetworkLayer(int neuronsCount, const std::string &activationFunctionName, double bias = 1);
	FFNetworkLayer(int neuronsCount, const std::string &activationFunctionName, int neuronInputs, double bias = 1);

	const ActivationFunction& getActivationFunction() const
	{
		return *activationFunction;
	}

	std::string getActivationFunctionName() const
	{
		return activationFunctionName;
	}
	const matrix& getOutput() const
	{
		return output;
	}
	const matrix& getWeights() const
	{
		return weights;
	}
	const matrix& getBiases() const
	{
		return biases;
	}
	matrix& getBiases()
	{
		return biases;
	}
	matrix& getWeights()
	{
		return weights;
	}
	matrix& getOutput() { return output; }

	void setActivationFunction(const std::string& activationFunctionName);
	void setWeights(const matrix &weights);
	void setBiases(const matrix &biases);

	int getLayerNumber() const
	{
		return layerNumber;
	}
	int getNeuronsCount() const
	{
		return neuronsCount;
	}
	int getNeuronInputs() const
	{
		return neuronInputs;
	}

	bool isHiddenLayer() const
	{
		return (next != nullptr && prev != nullptr);
	};
	bool isInputLayer() const
	{
		return prev == nullptr;
	}
	bool isOutputLayer() const
	{
		return next == nullptr;
	}
	const FFNeuralNetwork* getNetwork() const
	{
		return network;
	}
	FFNetworkLayer* getPreviousLayer()
	{
		return prev;
	}
	FFNetworkLayer* getNextLayer()
	{
		return next;
	}
	void randomizeWeights(const std::string& randomValueGeneratorName) { randomizeWeights(*RandomValueGeneratorFactory::instance().createActivationFunction(randomValueGeneratorName)); }
	void randomizeWeights(RandomValueGenerator& randomValueGenerator);
	void randomizeBiases(const std::string& randomValueGeneratorName) { randomizeBiases(*RandomValueGeneratorFactory::instance().createActivationFunction(randomValueGeneratorName)); }
	void randomizeBiases(RandomValueGenerator& randomValueGenerator);
private:
	const FFNeuralNetwork* network = nullptr;
	FFNetworkLayer* next = nullptr;
	FFNetworkLayer* prev = nullptr;
	ActivationFunction *activationFunction;
	std::string activationFunctionName;
	matrix weights; // [neuron][waga synapsy]
	matrix output; //X x 1
	matrix biases;
	int layerNumber;
	int neuronsCount;
	int neuronInputs;
};

