
#include "FFGradientDescend.h"
#include "ActivationFunction.h"

#include "CostFunction.h"
#include "FFNetworkLayer.h"
#include <algorithm>
#include "CostFunctionFactory.h"


FFGradientDescend::FFGradientDescend(FFNeuralNetwork& net, const std::string &costFunction) : 
	NetworkTrainer(), 
	network(net),
	costFunction(CostFunctionFactory::instance().createActivationFunction(costFunction))
{
}


matrix FFGradientDescend::trainingMethod(std::vector<std::reference_wrapper<matrix>> trInBatch,
	std::vector<std::reference_wrapper<matrix>> trExBatch)
{
	const int totalLayers = network.getTotalLayersCount();
	std::vector<matrix> batchWeightDeltas;
	std::vector<matrix> batchBiasDeltas;
	matrix totalDelta(network.getOutputLayer().getNeuronsCount(), 1);
	for (int i = 1; i < totalLayers; i++) {
		batchWeightDeltas.push_back(matrix(network.getLayer(i).getNeuronsCount(), network.getLayer(i).getNeuronInputs(), 0));
		batchBiasDeltas.push_back(matrix(network.getLayer(i).getNeuronsCount(), 1, 0));
	}
	for(int setInd = 0; setInd < trInBatch.size(); ++setInd)
	{
		matrix& trainingInput = trInBatch.at(setInd).get();
		matrix& trainingExpected = trExBatch.at(setInd).get();
		//Obliczenie wyjscia sieci dla pojedynczego zbioru trenujacego
		network.evaluate(trainingInput);

		//Obliczenie zmian wag dla ca³ej sieci
		const std::pair<std::vector<matrix>*, std::vector<matrix>*> biasWeightNetworkDelta = CalculateWeights(trainingInput, trainingExpected);
		std::vector<matrix>* networkWeightDeltas = biasWeightNetworkDelta.second;
		std::vector<matrix>* networkBiasDeltas = biasWeightNetworkDelta.first;

		for (int j = 0; j < batchWeightDeltas.size(); j++) {
			batchWeightDeltas[j] += networkWeightDeltas->at(j + 1); //Dodanie zmian wag sieci do ca³kowitej sumy zmian dla grupy
			batchBiasDeltas[j] += networkBiasDeltas->at(j + 1);
		}
	}
	for (int i = 1; i < network.getTotalLayersCount(); i++) {
		network.getLayer(i).getBiases() -= batchBiasDeltas[i - 1];
		network.getLayer(i).getWeights() -= batchWeightDeltas[i - 1];
	}
	for (int setInd = 0; setInd < trInBatch.size(); ++setInd)
	{
		matrix& trainingInput = trInBatch.at(setInd).get();
		matrix& trainingExpected = trExBatch.at(setInd).get();
		//Obliczenie wyjscia sieci dla pojedynczego zbioru trenujacego
		network.evaluate(trainingInput);

		//Dodanie b³êdu sieci do ca³kowitej sumy bledow
		matrix delta = network.getOutput() - trainingExpected;
		totalDelta += delta;
	}
	return std::move(totalDelta);
}

std::pair<std::vector<matrix>*, std::vector<matrix>*>  FFGradientDescend::CalculateWeights(matrix &input, matrix &expected)
{
	const int layersCount = network.getTotalLayersCount();
	std::vector<matrix>* networkWeightDeltas = new std::vector<matrix>(layersCount); //pochodna E po wagach * learningRate
	std::vector<matrix>* networkBiasDeltas = new std::vector<matrix>(layersCount); // pochodna E po wagach

	matrix previousDelta;
	for(int i = layersCount-1; i > 0; i--)
	{
		FFNetworkLayer& layer = network.getLayer(i);
		if(layer.isOutputLayer())
		{
			matrix costFuncDerivative = costFunction->mDerivativeCall(layer.getOutput(), expected);
			matrix outputActivationDerivative = layer.getActivationFunction().DerivativeCall(layer.getOutput());

			matrix outputDelta = matrix::Hadamard(costFuncDerivative, outputActivationDerivative); //delta
			previousDelta = outputDelta;
			
			matrix outputWeightsDelta = matrix::Multiply_BT(outputDelta, layer.getPreviousLayer()->getOutput()) * learningRate; //delta wag = delta * wejœcie warstwy i-tej
			if (previousNetworkWeightDeltas != nullptr) {
				previousNetworkWeightDeltas->at(i) *= momentum;
				outputWeightsDelta += previousNetworkWeightDeltas->at(i); //delta wag + poprzednia delta wag * momentum
			}

			(*networkBiasDeltas)[i] = outputDelta * learningRate;
			(*networkWeightDeltas)[i] = outputWeightsDelta;
		}
		else // warstwa ukryta
		{
			matrix a = matrix::Transpose(layer.getNextLayer()->getWeights()) * previousDelta;
			matrix outputActivationDerivative = layer.getActivationFunction().DerivativeCall(layer.getOutput());

			matrix layerDelta = matrix::Hadamard(a,outputActivationDerivative);
			previousDelta = layerDelta;

			matrix layerWeightsDelta = matrix::Multiply_BT(layerDelta, layer.getPreviousLayer()->getOutput()) * learningRate;
			if (previousNetworkWeightDeltas != nullptr) {
				previousNetworkWeightDeltas->at(i) *= momentum;
				layerWeightsDelta += previousNetworkWeightDeltas->at(i); //delta wag + poprzednia delta wag * momentum
			}

			(*networkBiasDeltas)[i] = layerDelta * learningRate;
			(*networkWeightDeltas)[i] = layerWeightsDelta;
		}
	}
	
	previousNetworkWeightDeltas = std::unique_ptr<std::vector<matrix>>(networkWeightDeltas);
	previousNetworkBiasDeltas = std::unique_ptr<std::vector<matrix>>(networkBiasDeltas);

	return std::make_pair(networkBiasDeltas, networkWeightDeltas);
}




