#pragma once
#include "FFNeuralNetwork.h"
#include <vector>
#include <map>
#include <string>
#include "../../matrix-library/include/matrix.h"

class matrix;


class NetworkTrainer
{
private:
	void initIndexTable(std::vector<int> &x);
	std::vector<int> trIndexTable;
	std::vector<int> vaIndexTable;
	int trSetIndex = 0;
	int valSetIndex = 0;
	int epochs = 0;
	double iterationRmsTrError = -1;
	double epochRmsTrError = -1;
	double epochTrErrorSum = 0;
	bool randomizeTrainingData = false;
	int batchSize = 1;
	void randomizeTraining();
protected:
    std::vector<matrix> trainingExpectedOutput;
	std::vector<matrix> trainingInput;
	std::vector<matrix> validationExpectedOutput;
	std::vector<matrix> validationInput;
	std::vector<int> indexesTrainingInput;
	std::vector<int> indexesTrainingExpectedOutput;
	
	virtual matrix trainingMethod(const std::vector<std::reference_wrapper<matrix>> trInBatch, const std::vector<std::reference_wrapper<matrix>> trExBatch) = 0;
	NetworkTrainer() = default;
public:
	void iteration();
	void epoch();
	void setTrainingSet(matrix input[], int inputLength, matrix expectedOutput[], int expectedLength);
	void setValidationSet(matrix input[], int inputLength, matrix expectedOutput[], int expectedLength);
	void setTrainingSet(std::vector<matrix>& input, std::vector<matrix>& expected);
	void setValidationSet(std::vector<matrix>& input, std::vector<matrix>& expected);
	void setRandomizeTrainingData(bool value) { randomizeTrainingData = value; }
	std::vector<matrix> getTrainingInput();
	std::vector<matrix> getTrainingExpectedOutput();
	void setEpochOutputInInitialOrder(std::vector<matrix> &epochOutput);
	int epochsCount() const { return epochs; }
	double lastIterationTrainingError() const { return  iterationRmsTrError; }
	double lastEpochTrainingError() const { return epochRmsTrError; }
	void setBatchSize(int sz) { if(sz > 0) batchSize = sz; }
	int getBatchSize() const { return batchSize; }
	virtual void reset();
	virtual ~NetworkTrainer() = default;
};



