#pragma once

#include <QObject>
#include <QEvent>
#include "CommandProxy.h"


class NetworkContext;
class matrix;
class CommandThread;
class FFNeuralNetwork;
enum CommandType;

class NetworkTrainerProxy : public CommandProxy
{
	Q_OBJECT
private:
	NetworkContext &context;
	bool isTraining = false; //TODO bez
	CommandThread* thread;
	double errorThreshold;
	bool checkErrorThreshold;
public:
	NetworkTrainerProxy(NetworkContext &context, QObject*parent);
	~NetworkTrainerProxy() = default;
	void stop();
	void terminate();
	bool acceptedObject(QString objectName) override { return (objectName == "trainer" || objectName == "") ? true : false; }
	void stopAsyncCommands() override { stop(); }
	bool validateTrainingSet();
public slots:
	CommandType Iteration();
	CommandType Epoch();
	CommandType CheckThreshold(QString check, QString value = "0");
	CommandType Train(QString epochsCount);
	CommandType SetRandomizeTrainingData(bool value);
	//CommandType SetTrainer(QString trainer);

};
