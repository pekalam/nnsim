#pragma once
#include <vector>
#include <memory>

class FFNetworkLayer;
class ActivationFunction;
class matrix;

class FFNeuralNetwork
{
private:
	std::vector<std::shared_ptr<FFNetworkLayer>> layers;
public:
	FFNeuralNetwork() = default;
	matrix& evaluate(const matrix& input);
	void addLayer(const FFNetworkLayer& layer);
	FFNeuralNetwork& operator<<(const FFNetworkLayer& layer) { addLayer(layer); return *this; }
	void insertLayer(int index, const FFNetworkLayer &layer);
	void replaceLayer(int index, const FFNetworkLayer &layer);
	

	int getNeuronsCount() const;
	FFNetworkLayer& getOutputLayer();
	FFNetworkLayer& getInputLayer();
	FFNetworkLayer& getLayer(int layerNum) { return *layers[layerNum]; }
	matrix& getOutput() const;
	int getTotalLayersCount() const;
	int getHiddenLayersCount() const;
	void reset() { layers.clear(); }
	void randomizeLayersWeights(const std::string& randomValueGeneratorName);
	void randomizeLayersBiases(const std::string& randomValueGeneratorName);
};

