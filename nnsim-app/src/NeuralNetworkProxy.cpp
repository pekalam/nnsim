#include "NeuralNetworkProxy.h"
#include "FFNetworkLayer.h"
#include "FFNeuralNetwork.h"
#include <qfile.h>
#include "CommandType.h"
#include "NetworkContext.h"
#include "CommandExecutor.h"

const std::map<QString, int> NeuralNetworkProxy::RandomizationAlgorithm = { {"Range",0}, {"Normal",1}, {"None", 2} };

NeuralNetworkProxy::NeuralNetworkProxy(NetworkContext &context, QObject*parent) :
CommandProxy(parent), context(context)
{
}

inline bool NeuralNetworkProxy::strToBool(QString str)
{
	return str == "true";
}

CommandType NeuralNetworkProxy::Clear()
{
	context.inputDataTraining.clear();
	context.idealDataTraining.clear();
	context.network.reset();
	return NETWORK_CLEAR;
}

CommandType NeuralNetworkProxy::AddLayer(QString neuronsCount,
                                              QString activationFunction)
{
	context.network << FFNetworkLayer(neuronsCount.toInt(), activationFunction.toStdString());
	return  NETWORK_STRUCTURE;
}

CommandType NeuralNetworkProxy::Evaluate()
{
	if (chosenInputIndex >= 0 && context.inputDataTraining.size() > 0 
		&& context.network.getInputLayer().getNeuronsCount() == context.inputDataTraining.at(chosenInputIndex).rows())
		context.network.evaluate(context.inputDataTraining.at(chosenInputIndex));
	else
	{
		commandError = "Nieprawid³owe dane wejœciowe dla sieci";
		return INVALID_COMMAND;
	}
	return NETWORK_VALUE;
}

CommandType NeuralNetworkProxy::ChangeActivationFunction(QString layerNumber,
                                                         QString activationFunction)
{
	bool ok;
	const int layerIndex = layerNumber.toInt(&ok);
	if(!ok || layerIndex < 0 || layerIndex >= context.network.getTotalLayersCount())
	{
		commandError = "B³êdny parametr";
		return INVALID_COMMAND;
	}
	context.network.getLayer(layerIndex).setActivationFunction(activationFunction.toStdString());
	return NETWORK_STRUCTURE;
}

CommandType NeuralNetworkProxy::addSet(QString setName, QString index, QString set)
{
	bool ok;
	int ind = index.toInt(&ok);
	if (!ok) {
		commandError = "Podano b³êdn¹ wartoœæ indeksu";
		return INVALID_COMMAND;
	}
	setName = setName.trimmed();
	set = set.trimmed();
	set.remove(QRegExp("\\s+"));
	QStringList number_list = set.split(',');
	matrix m(number_list.length(), 1);
	int i = 0;
	for (QString num : number_list)
	{
		double x = num.toDouble(&ok);
		if (!ok)
		{
			commandError = "Nie mozna wczytac podanego zbioru";
			return INVALID_COMMAND;
		}
		MX_R1_AS_VEC(i, m) = x;
		i++;
	}
	if (setName == "input") {
		if(ind < context.inputDataTraining.size())
			context.inputDataTraining[ind] = m;
	}
	else {
		if(ind < context.idealDataTraining.size())
			context.idealDataTraining[ind] = m;
	}
	return DATA;
}

CommandType NeuralNetworkProxy::chooseNetworkInputIndex(QString source, QString index)
{
	bool ok = false;
	if (source == "input")
	{
		const int ind = index.toInt(&ok);
		if (ind < 0 || !ok) {
			commandError = "Podano bledna wartosc indeksu";
			return INVALID_COMMAND;
		}
		else {
			chosenInputIndex = ind;
			for(int i = 0; i < context.network.getInputLayer().getNeuronsCount(); i++)
			{
				MX_R1_AS_VEC(i,context.network.getInputLayer().getOutput()) = MX_R1_AS_VEC(i,context.inputDataTraining.at(ind));
			}
		}
	}
	else {
		commandError = QString("Obiekt %1 nie istnieje").arg(source);
		return INVALID_COMMAND;
	}
	return NETWORK_INPUT;
}

CommandType NeuralNetworkProxy::addSetFromFile(QString setName, QString index, QString fname)
{
	QFile f(fname);

	f.close();
	return NETWORK_INPUT;
}

CommandType NeuralNetworkProxy::objectInfo(QString object, CommandExecutor &cmdEngine)
{
	QString r;
	if (object == "input" || object == "ideal")
	{
		std::vector<matrix> *data;
		if (object == "ideal")
			data = &context.idealDataTraining;
		else
			data = &context.inputDataTraining;
		for (matrix element : *data)
		{
			r += "[ ";
			for (int i = 0; i < element.rows(); i++)
				r += QString("%1; ").arg(MX_R1_AS_VEC(i, element));
			r += "] ";
		}
		
	}
	else if(object == "net")
	{
		r += QString("\nLayers: %1\nNeurons: %2")
			.arg(context.network.getTotalLayersCount())
			.arg(context.network.getNeuronsCount());
	}
	cmdEngine.terminalOutput(r);
	return IGNORE_CMD;
}

CommandType NeuralNetworkProxy::app_Evaluate(const matrix& input)
{
	if (context.network.getInputLayer().getNeuronsCount() == input.rows())
		context.network.evaluate(input);
	else
	{
		commandError = "Nieprawid³owe dane wejœciowe dla sieci";
		return INVALID_COMMAND;
	}
	return NETWORK_VALUE;
}
