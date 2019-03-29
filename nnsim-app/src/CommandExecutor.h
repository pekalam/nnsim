#pragma once
#include <QObject>
#include "ThreadSafe.h"

#include "NetworkTrainerProxy.h"
#include "VisualizationWindowProxy.h"
#include "NeuralNetworkProxy.h"
#include "CommandProxy.h"
#include <functional>
#include "CommandHandler.h"
#include <QRegularExpressionMatch>

#define CMD_REGEX "(net|trainer|ideal|input)?(?:\\.)?(?:(\\w+)(?:\\((?:\\s*(\\w+)\\s*)?(?:\\,\\s*([\\w|\\.]+)\\s*)?(?:\\,\\s*(\\w+)\\s*)?(?:\\,\\s*(\\w+)\\s*)?\\)))?(?:\\[(\\d+)\\]\\s*(=)\\s*\\{(.*)\\})?(?:(\\w+)\\s*(=)\\s*(\\w+)(?:\\[(\\d+)\\]))?(\\w+)?"

#define REGEX_OBJ "(\\w+)\\.(\\w+)"
#define REGEX_OP "(\\(\\s*(\\w+)?\\,?(\\w+)?\\,?(\\w+)?\\,?(\\w+)?\\))|(\\[(\\d+)\\](.*))|(\\s*=\\s*(.*))"
#define REGEX_ASSIGNVAL "((\\w+)\\[(\\d+)\\])|({(.*)})"

class NetworkContext;
class FFNeuralNetwork;
class FFGradientDescend;


class CommandExecutor : public QObject
{
	Q_OBJECT
public:
	CommandExecutor(NetworkContext &context,  QObject *parent);
	void addProxy(CommandProxy& proxy);
	void removeProxy(CommandProxy &proxy);
	void addHandler(CommandHandler& proxy);
	void removeHandler(CommandHandler &proxy);
	void handleCommand(CommandType cmd);
private:
	std::vector<std::reference_wrapper<CommandProxy>> commandProxies;
	std::vector<std::reference_wrapper<CommandHandler>> commandHandlers;
	
	CommandProxy& getProxyForObj(QString objName);
	QRegularExpression cmdRegex;
public slots:
	void executeCommand(QString cmd);
	void stopAsyncCommands();
signals:
	void terminalOutput(QString msg);
};

