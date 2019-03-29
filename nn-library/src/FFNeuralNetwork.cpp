#include "FFNeuralNetwork.h"
#include "ActivationFunction.h"
#include "../../matrix-library/include/matrix.h"
#include "FFNetworkLayer.h"

matrix& FFNeuralNetwork::evaluate(const matrix& input)
{
	for (int i = 0; i < layers[0]->neuronsCount; ++i)
		MX_R1_AS_VEC(i, layers[0]->output) = MX_R1_AS_VEC(i, input);

	for (int i = 1; i < (int)layers.size(); ++i) {
		int Wr = layers[i]->neuronsCount; int Wc = layers[i]->neuronInputs;
		int Xr = layers[i-1]->neuronsCount;
		if (Xr != Wc) {
			return *(new matrix(0, 0));
		}
		// Y = f(W*X(i-1) + bias)
		matrix Y = matrix::Multiply(layers[i]->weights, layers[i-1]->output);
		matrix::Add_inp(Y, layers[i]->biases); //Y = Y + bias
		// Y = g(f(W*X(i-1)))
		layers[i]->activationFunction->Call(Y);
		layers[i]->output = std::move(Y);
	}

	return layers[layers.size() - 1]->output;
}

void FFNeuralNetwork::addLayer(const FFNetworkLayer& layer)
{
	FFNetworkLayer *newLayer = new FFNetworkLayer(layer);
	const int neuronSynapses = layers.size() > 0 ? layers.back()->neuronsCount : 1;
	if (layer.weights.cols() != neuronSynapses) {
		newLayer->weights.reshape(layer.neuronsCount, neuronSynapses);
		newLayer->neuronInputs = newLayer->weights.cols();
	}
	newLayer->layerNumber = layers.size();
	newLayer->network = this;
	newLayer->randomizeWeights("01");
	layers.push_back(std::shared_ptr<FFNetworkLayer>(newLayer));
	const int layersCount = layers.size();
	for (int i = 1; i < layersCount; ++i)
	{
		layers[i-1]->next = layers[i].get();
		layers[i]->prev = layers[i - 1].get();
	}
}

void FFNeuralNetwork::insertLayer(int index, const FFNetworkLayer& layer)
{
	if (layers.size() == 0 || index > layers.size() || index < 0)
		return;
	if(index == layers.size())
	{
		addLayer(layer);
	}
	else if(index == 0)
	{
		FFNetworkLayer *newLayer = new FFNetworkLayer(layer);
		FFNetworkLayer* layer0 = layers[0].get();
		newLayer->weights.reshape(0, 0);
		newLayer->neuronInputs = 0;
		newLayer->layerNumber = 0;
		newLayer->network = this;
		newLayer->prev = nullptr;
		newLayer->randomizeWeights("01");
		layers.insert(layers.begin(), std::shared_ptr<FFNetworkLayer>(newLayer));
		
		layer0->layerNumber = 1;
		layer0->weights.reshape(layer0->neuronsCount, newLayer->neuronsCount);
		layer0->neuronInputs = newLayer->neuronsCount;
		const int layersCount = layers.size();
		for (int i = 1; i < layersCount; i++)
		{
			layers[i - 1]->next = layers[i].get();
			layers[i]->prev = layers[i - 1].get();
		}
	}
	else
	{
		
		FFNetworkLayer *nextLayer = layers[index].get();
		FFNetworkLayer *prevLayer = layers[index-1].get();
		FFNetworkLayer *newLayer = new FFNetworkLayer(layer);
		layers.insert(layers.begin()+index, std::shared_ptr<FFNetworkLayer>(newLayer));
		newLayer->weights.reshape(newLayer->neuronsCount, prevLayer->neuronsCount);
		nextLayer->weights.reshape(nextLayer->neuronsCount, newLayer->neuronsCount);
		nextLayer->neuronInputs = newLayer->neuronsCount;
		newLayer->neuronInputs = prevLayer->neuronsCount;
		newLayer->layerNumber = index;
		newLayer->network = this;
		newLayer->randomizeWeights("01");
		const int layersCount = layers.size();
		layers[0]->layerNumber = 0;
		for (int i = 1; i < layersCount; i++)
		{
			layers[i - 1]->next = layers[i].get();
			layers[i]->prev = layers[i - 1].get();
			layers[i]->layerNumber = i;
		}
	}
}

void FFNeuralNetwork::replaceLayer(int index, const FFNetworkLayer& layer)
{
	if (layers.size() <= 0 || index >= layers.size() || index < 0)
		return;
	FFNetworkLayer *newLayer = new FFNetworkLayer(layer);
	if (index == 0)
	{
		newLayer->weights.reshape(0, 0);
		newLayer->neuronInputs = 0;
		newLayer->layerNumber = 0;
		newLayer->network = this;
		newLayer->prev = nullptr;
		newLayer->randomizeWeights("01");
		layers[index] = std::shared_ptr<FFNetworkLayer>(newLayer);
		const int layersCount = layers.size();
		if(layersCount > 1)
		{
			FFNetworkLayer *nextLayer = layers[index+1].get();
			nextLayer->weights.reshape(nextLayer->neuronsCount, layer.neuronsCount);
			nextLayer->prev = layers[index].get();
			newLayer->next = layers[index + 1].get();
		}
	}
	else
	{
		FFNetworkLayer *prevLayer = layers[index - 1].get();
		newLayer->weights.reshape(newLayer->neuronsCount, prevLayer->neuronsCount);
		newLayer->neuronInputs = prevLayer->neuronsCount;
		newLayer->layerNumber = index;
		newLayer->network = this;
		newLayer->randomizeWeights("01");
		layers[index] = std::shared_ptr<FFNetworkLayer>(newLayer);
		prevLayer->next = layers[index].get();
		if (index + 1 < layers.size())
		{
			FFNetworkLayer *nextLayer = layers[index + 1].get();
			nextLayer->weights.reshape(nextLayer->neuronsCount, layer.neuronsCount);
			nextLayer->prev = layers[index].get();
			newLayer->next = layers[index + 1].get();
		}
	}
}

int FFNeuralNetwork::getNeuronsCount() const
{
	int t = 0;
	for (int i = 0; i < getTotalLayersCount(); i++)
		t += layers[i]->neuronsCount;
	return t;
}

FFNetworkLayer& FFNeuralNetwork::getOutputLayer()
{
	return *layers[layers.size() - 1];
}

FFNetworkLayer& FFNeuralNetwork::getInputLayer()
{
	return *layers[0];
}

matrix& FFNeuralNetwork::getOutput() const
{
	return layers[layers.size() - 1]->output;
}

int FFNeuralNetwork::getTotalLayersCount() const
{
	return layers.size();
}

int FFNeuralNetwork::getHiddenLayersCount() const
{
	if (layers.size() - 2 >= 0)
		return layers.size() - 2;
	else
		return 0;
}

void FFNeuralNetwork::randomizeLayersWeights(const std::string& randomValueGeneratorName)
{
	for(std::shared_ptr<FFNetworkLayer> layer : layers)
	{
		layer->randomizeWeights(randomValueGeneratorName);
	}
}

void FFNeuralNetwork::randomizeLayersBiases(const std::string& randomValueGeneratorName)
{
	for (std::shared_ptr<FFNetworkLayer> layer : layers)
	{
		layer->randomizeBiases(randomValueGeneratorName);
	}
}
