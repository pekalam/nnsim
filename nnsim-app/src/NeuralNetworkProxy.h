#pragma once


#include <qobjectdefs.h>
#include "ActivationFunction.h"
#include "FFNetworkLayer.h"
#include "CommandProxy.h"


class CommandExecutor;
enum CommandType;
class NetworkContext;

class NeuralNetworkProxy : public CommandProxy
{
	Q_OBJECT
public:
	NeuralNetworkProxy(NetworkContext &context, QObject*parent = Q_NULLPTR);
	bool acceptedObject(QString objectName) override { return (objectName == "net" || objectName == "input" || objectName == "ideal" || objectName == "") ? true : false; }
private:
	NetworkContext &context;
	int chosenInputIndex = -1;
	static inline bool strToBool(QString str);
	static const std::map<QString, int> RandomizationAlgorithm;
public slots:
	CommandType Clear();
	CommandType gClear() { return Clear(); }
	CommandType AddLayer(QString neuronsCount, QString activationFunction);
	CommandType gAddLayer(QString neuronsCount, QString activationFunction) { return AddLayer(neuronsCount, activationFunction); }
	CommandType AddInputLayer(QString neuronsCount, QString activationFunction){ return AddLayer(neuronsCount, activationFunction); }
	CommandType gAddInputLayer(QString neuronsCount, QString activationFunction) { return AddLayer(neuronsCount, activationFunction); }
	CommandType Evaluate();
	CommandType gEvaluate() { return Evaluate(); }
	CommandType ChangeActivationFunction(QString layerNumber, QString activationFunction);
	CommandType gChangeActivationFunction(QString layerNumber, QString activationFunction) { return ChangeActivationFunction(layerNumber, activationFunction); }
	CommandType addSet(QString setName, QString index, QString set);
	CommandType chooseNetworkInputIndex(QString source, QString index);
	CommandType addSetFromFile(QString setName, QString index, QString fname);
	CommandType objectInfo(QString object, CommandExecutor &cmdEngine);
	CommandType app_Evaluate(const matrix& input);
};


