#include "NetworkContext.h"
#include <map>
#include "FFGradientDescend.h"
NetworkContext::NetworkContext() :
                trainer(new FFGradientDescend(network, "Quadratic")),
				networkType(FeedForwardNetwork),
				networkTaskType(Regression)
{
}

void NetworkContext::clear()
{
	network.reset();
	inputDataTraining.clear();
	idealDataTraining.clear();
	inputDataValidation.clear();
	idealDataValidation.clear();
	previousEpochOutput.clear();
	previousEpochInput.clear();
}
