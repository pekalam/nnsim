#pragma once
#include "NetworkTrainer.h"
#include <string>
class CostFunction;
class matrix;


class FFGradientDescend : public NetworkTrainer
{
protected:
	matrix trainingMethod(std::vector<std::reference_wrapper<matrix>> trInBatch,
		std::vector<std::reference_wrapper<matrix>> trExBatch) override;
private:
	// Lista zmian wag dla wszystkich warstw sieci
	std::unique_ptr<std::vector<matrix>> previousNetworkWeightDeltas = nullptr;
	// Lista zmian obci¹¿enia dla wszystkich warstw sieci
	std::unique_ptr<std::vector<matrix>> previousNetworkBiasDeltas = nullptr;
	// Funkcja b³êdu
	CostFunction* costFunction;
	//Oblicza zmianê wag dla ca³ej sieci
	std::pair<std::vector<matrix>*, std::vector<matrix>*> CalculateWeights(matrix &input, matrix &expectedOutput);
	FFNeuralNetwork &network;
public:
	FFGradientDescend(FFNeuralNetwork& net, const std::string &costFunction);
	FFGradientDescend(FFGradientDescend &c) = default;
	FFGradientDescend(FFGradientDescend &&c) = default;
	
	double momentum = 0.9;
	double learningRate = 0.005;
	int batchSize = 1;
};

