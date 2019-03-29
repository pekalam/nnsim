#pragma once
#include <vector>
#include "ThreadSafe.h"
#include "FFNeuralNetwork.h"


class matrix;

class FFGradientDescend;

class NetworkContext
{
public:
	enum NetworkType { FeedForwardNetwork };
	enum NetworkTaskType { Regression, Classification };
	NetworkContext();
	FFNeuralNetwork network;
	NetworkType networkType;
	NetworkTaskType networkTaskType;
	ThreadSafe<FFGradientDescend*> trainer;
	std::vector<matrix> inputDataTraining;
	std::vector<matrix> idealDataTraining;
	std::vector<matrix> inputDataValidation;
	std::vector<matrix> idealDataValidation;
	std::vector<matrix> previousEpochOutput;
	std::vector<matrix> previousEpochInput;
	void setNetworkType(NetworkType type) { networkType = type; }
	NetworkType getNetworkType() const { return networkType; }
	void setNetworkTaskType(NetworkTaskType type) { networkTaskType = type; }
	NetworkTaskType getNetworkTaskType() const { return networkTaskType; }
	void clear();
};

