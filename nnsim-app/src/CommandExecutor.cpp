#include "CommandExecutor.h"
#include <QRegularExpressionMatch>
#include <qmetaobject.h>
#include <QVector>
#include "CommandType.h"
#include "CommandHandler.h"

CommandExecutor::CommandExecutor(NetworkContext &context, QObject *parent)
: QObject(parent), cmdRegex(CMD_REGEX)
{
}

void CommandExecutor::addProxy(CommandProxy& proxy)
{
	commandProxies.push_back(proxy);
	QObject::connect(&proxy, &CommandProxy::asyncCommandCompleted, this, &CommandExecutor::handleCommand);
}

void CommandExecutor::addHandler(CommandHandler& handler)
{
	commandHandlers.push_back(handler);
}

void CommandExecutor::removeHandler(CommandHandler& proxy)
{
	for (auto i = commandHandlers.begin(); i != commandHandlers.end(); ++i)
		if (&i->get() == &proxy) {
			commandHandlers.erase(i);
			break;
		}
}


void CommandExecutor::handleCommand(CommandType cmd)
{
	for (std::reference_wrapper<CommandHandler> handler : commandHandlers) {
		handler.get().handleCommand(cmd);
	}
}

CommandProxy& CommandExecutor::getProxyForObj(QString objName)
{
	for (std::reference_wrapper<CommandProxy> proxy : commandProxies) {
		if (proxy.get().acceptedObject(objName))
			return proxy.get();
	}
}

/*
 Wykonuje komendê podan¹ jako tekst
 */
void CommandExecutor::executeCommand(QString cmd)
{
	cmd = cmd.trimmed();
	bool ok = false;
	CommandType cmdType = INVALID_COMMAND;

	QRegularExpressionMatch cmdMatch = cmdRegex.match(cmd);
	if (!cmdMatch.hasMatch())
		emit terminalOutput("Nieprawidlowe polecenie");

	const QString object = cmdMatch.captured(1);
	if (cmdMatch.lastCapturedIndex() == 13)
	{
		const QString objMember = cmdMatch.captured(10);
		if (object == "net") //TODO: operator przypisania
		{
			const QString sourceObjName = cmdMatch.captured(12);
			const QString sourceIndex = cmdMatch.captured(13);
			ok = QMetaObject::invokeMethod(&getProxyForObj(object), "chooseNetworkInputIndex", Qt::DirectConnection, Q_RETURN_ARG(CommandType, cmdType), Q_ARG(QString, sourceObjName), Q_ARG(QString, sourceIndex));
			if (!ok)
				goto cmdError;
			if (cmdType == INVALID_COMMAND)
				emit terminalOutput(getProxyForObj(object).commandError);
			else
				handleCommand(cmdType);
			return;
		}
	}
	else if (cmdMatch.lastCapturedIndex() == 9 || cmdMatch.lastCapturedIndex() == 1)
	{
		if (cmdMatch.lastCapturedIndex() == 9)
		{
			const QString index_str = cmdMatch.captured(7);
			const QString set_str = cmdMatch.captured(9);

			ok = QMetaObject::invokeMethod(&getProxyForObj(object), "addSet", Qt::DirectConnection, Q_RETURN_ARG(CommandType, cmdType), Q_ARG(QString, object), Q_ARG(QString, index_str), Q_ARG(QString, set_str));
		}
		else if (cmdMatch.lastCapturedIndex() == 1) //input | ideal
		{
			ok = QMetaObject::invokeMethod(&getProxyForObj(object), "objectInfo", Qt::DirectConnection, Q_RETURN_ARG(CommandType, cmdType), Q_ARG(QString, object), Q_ARG(CommandExecutor&, *this));
			return;
		}
		if (!ok)
			goto cmdError;
		if (cmdType == INVALID_COMMAND)
			emit terminalOutput(getProxyForObj(object).commandError);
		else
			handleCommand(cmdType);
		return;
	}
	else
	{
		std::string functionName = cmdMatch.captured(2).toStdString();
		for (std::reference_wrapper<CommandProxy> proxy : commandProxies) {
			if (!proxy.get().acceptedObject(object))
				continue;
			if (cmdMatch.lastCapturedIndex() == 2) {// 0 params
				ok = QMetaObject::invokeMethod(&proxy.get(), functionName.c_str(), Qt::DirectConnection, Q_RETURN_ARG(CommandType, cmdType));
			}
			else if (cmdMatch.lastCapturedIndex() == 3) //1 param
			{
				ok = QMetaObject::invokeMethod(&proxy.get(), functionName.c_str(), Qt::DirectConnection, Q_RETURN_ARG(CommandType, cmdType), Q_ARG(QString, cmdMatch.captured(3)));
			}
			else if (cmdMatch.lastCapturedIndex() == 4) //2 params
			{
				ok = QMetaObject::invokeMethod(&proxy.get(), functionName.c_str(), Qt::DirectConnection, Q_RETURN_ARG(CommandType, cmdType), Q_ARG(QString, cmdMatch.captured(3)), Q_ARG(QString, cmdMatch.captured(4)));
			}
			else if (cmdMatch.lastCapturedIndex() == 5) //3 params
			{
				ok = QMetaObject::invokeMethod(&proxy.get(), functionName.c_str(), Qt::DirectConnection, Q_RETURN_ARG(CommandType, cmdType), Q_ARG(QString, cmdMatch.captured(3)), Q_ARG(QString, cmdMatch.captured(4)), Q_ARG(QString, cmdMatch.captured(5)));
			}
			if (!ok)
				goto cmdError;
			if (cmdType == INVALID_COMMAND) {
				emit terminalOutput(proxy.get().commandError);
				return;
			}
			else {
				handleCommand(cmdType); return;
			}
		}
	}

	cmdError:
	emit terminalOutput("Nieprawidlowe polecenie");

}

void CommandExecutor::stopAsyncCommands()
{
	for (std::reference_wrapper<CommandProxy> proxy : commandProxies) {
		proxy.get().stopAsyncCommands();
	}
}


