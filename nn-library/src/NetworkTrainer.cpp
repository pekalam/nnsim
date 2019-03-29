#include "NetworkTrainer.h"
#include <time.h>
#include <algorithm>
#include "../../matrix-library/include/matrix.h"

void NetworkTrainer::iteration()
{
	if (trSetIndex == 0)
	{
		epochTrErrorSum = 0;
		if (randomizeTrainingData)
			randomizeTraining();
	}
	

	const std::vector<std::reference_wrapper<matrix>> trInBatch(&trainingInput[trSetIndex],
	                                                            &trainingInput[trSetIndex] + batchSize);
	const std::vector<std::reference_wrapper<matrix>> trExBatch(&trainingExpectedOutput[trSetIndex],
	                                                            &trainingExpectedOutput[trSetIndex] + batchSize);

	matrix error = trainingMethod(trInBatch, trExBatch);

	matrix::Pow_inp(error, 2);
	const int expectedTrSetSize = trainingExpectedOutput.at(0).rows();
	const double iterationError = matrix::Sum(error);
	iterationRmsTrError = sqrt(iterationError / (expectedTrSetSize * trExBatch.size()));
	epochTrErrorSum += iterationRmsTrError;


	if (trSetIndex == trainingInput.size() - 1) {
		trSetIndex = 0;
		epochRmsTrError = sqrt(epochTrErrorSum / trainingInput.size());
		epochs++;
	}else
	{
		trSetIndex++;
	}
}

void NetworkTrainer::epoch()
{
	const int currentEpoch = epochs;
	while (currentEpoch == epochs)
	{
		iteration();
	}
	
}

void NetworkTrainer::setTrainingSet(matrix input[], int inputLength, matrix expectedOutput[], int expectedLength)
{
	trainingInput.clear();
	trainingInput.insert(trainingInput.end(), &input[0], &input[inputLength]);
	trainingExpectedOutput.clear();
	trainingExpectedOutput.insert(trainingExpectedOutput.end(), &expectedOutput[0], &expectedOutput[expectedLength]);

	indexesTrainingInput.clear(); indexesTrainingInput.resize(inputLength); initIndexTable(indexesTrainingInput);
	indexesTrainingExpectedOutput.clear(); indexesTrainingExpectedOutput.resize(expectedLength); initIndexTable(indexesTrainingExpectedOutput);
}

void NetworkTrainer::setValidationSet(matrix input[], int inputLength, matrix expectedOutput[], int expectedLength)
{
	validationInput.clear();
	validationInput.insert(validationInput.end(), &input[0], &input[inputLength]);
	validationExpectedOutput.clear();
	validationExpectedOutput.insert(validationExpectedOutput.end(), &expectedOutput[0], &expectedOutput[expectedLength]);
}

void NetworkTrainer::setTrainingSet(std::vector<matrix>& input, std::vector<matrix>& expected)
{
	trainingInput = input;
	trainingExpectedOutput = expected;
	indexesTrainingInput.clear(); indexesTrainingInput.resize(input.size()); initIndexTable(indexesTrainingInput);
	indexesTrainingExpectedOutput.clear(); indexesTrainingExpectedOutput.resize(expected.size()); initIndexTable(indexesTrainingExpectedOutput);
}

void NetworkTrainer::setValidationSet(std::vector<matrix>& input, std::vector<matrix>& expected)
{
	validationInput = input;
	validationExpectedOutput = expected;
}


void NetworkTrainer::initIndexTable(std::vector<int> &x) {
	for (int i = 0; i < x.size(); i++)
		x[i] = i;
}

void NetworkTrainer::randomizeTraining()
{
	std::vector<matrix> initialTrainingInput(trainingInput.size());
	std::vector<matrix> initialTrainingExpectedOutput(trainingExpectedOutput.size());
	for (int i = 0; i < trainingInput.size(); i++) {
		initialTrainingInput[indexesTrainingInput[i]] = trainingInput[i];
		initialTrainingExpectedOutput[indexesTrainingExpectedOutput[i]] = trainingExpectedOutput[i];
	}
	initIndexTable(indexesTrainingInput);
	initIndexTable(indexesTrainingExpectedOutput);
	for (int i = 0; i < trainingInput.size(); i++)
	{
		std::random_shuffle(indexesTrainingInput.begin(), indexesTrainingInput.end());
		std::random_shuffle(indexesTrainingExpectedOutput.begin(), indexesTrainingExpectedOutput.end());
	}
	std::vector<matrix> newTrainingInput(trainingInput.size());
	std::vector<matrix> newTrainingExpectedOutput(trainingExpectedOutput.size());
	for (int i = 0; i < trainingInput.size(); i++) {
		newTrainingInput[i] = initialTrainingInput[indexesTrainingInput[i]];
		newTrainingExpectedOutput[i] = initialTrainingExpectedOutput[indexesTrainingExpectedOutput[i]];
	}
	trainingInput = newTrainingInput;
	trainingExpectedOutput = newTrainingExpectedOutput;
}

void NetworkTrainer::setEpochOutputInInitialOrder(std::vector<matrix> &epochOutput) {
	std::vector<matrix> r(epochOutput.size());
	for (int i = 0; i < indexesTrainingExpectedOutput.size(); i++)
		r[indexesTrainingExpectedOutput[i]] = epochOutput[i];
	epochOutput = r;
}

void NetworkTrainer::reset()
{
	epochs = 0;
	iterationRmsTrError = -1;
	epochRmsTrError = -1;
	epochTrErrorSum = 0;
	validationInput.clear();
	validationExpectedOutput.clear();
	trainingInput.clear();
	trainingExpectedOutput.clear();
}

std::vector<matrix> NetworkTrainer::getTrainingInput()
{
	std::vector<matrix> r(trainingInput.size());
	for (int i = 0; i < indexesTrainingInput.size(); i++)
		r[indexesTrainingInput[i]] = trainingInput[i];
	return r;
}

std::vector<matrix> NetworkTrainer::getTrainingExpectedOutput()
{
	std::vector<matrix> r(trainingExpectedOutput.size());
	for (int i = 0; i < indexesTrainingExpectedOutput.size(); i++)
		r[indexesTrainingExpectedOutput[i]] = trainingExpectedOutput[i];
	return r;
}

