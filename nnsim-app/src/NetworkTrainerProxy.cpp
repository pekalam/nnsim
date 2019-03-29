#include "NetworkTrainerProxy.h"
#include <QtConcurrent>
#include "CommandType.h"
#include "CommandThread.h"
#include "VisWindow.h"
#include "FFGradientDescend.h"
#include "NetworkContext.h"

NetworkTrainerProxy::NetworkTrainerProxy(NetworkContext &context, QObject*parent) : CommandProxy(parent), context(context), thread(new CommandThread(this)), errorThreshold(0), checkErrorThreshold(false)
{
	bool ok;
	ok = connect(thread, &CommandThread::completed, this, &NetworkTrainerProxy::asyncCommandCompleted, Qt::BlockingQueuedConnection);
	Q_ASSERT(ok);
}

void NetworkTrainerProxy::stop()
{
	isTraining = false;
}

void NetworkTrainerProxy::terminate()
{
	if (isTraining)
		thread->thread.terminate();
}

bool NetworkTrainerProxy::validateTrainingSet()
{
	if(context.inputDataTraining.size() > 0 && context.idealDataTraining.size() > 0)
	{
		if (context.getNetworkTaskType() == NetworkContext::NetworkTaskType::Regression)
			return true;
		else if (context.getNetworkTaskType() == NetworkContext::NetworkTaskType::Classification)
			return true; 
	}
	return false;
}

CommandType NetworkTrainerProxy::Iteration()
{
	if (isTraining) {
		commandError = "Polecenie jest w trakcie wykonywania";
		return INVALID_COMMAND;
	}
	if(!validateTrainingSet())
	{
		commandError = "Nieprawid³owe dane wejœciowe";
		return INVALID_COMMAND;
	}
	context.trainer.get()->setTrainingSet(context.inputDataTraining, context.idealDataTraining);
	
	this->thread->setFunction([&](CommandThread* th) {
		-context.trainer;
			const int currentEpoch = context.trainer.get()->epochsCount();
			context.trainer.get()->iteration();
			isTraining = false;
			const int epoch = context.trainer.get()->epochsCount();
		+context.trainer;
		if (epoch != currentEpoch)
			return TRAINER_EPOCH;
		else 
			return TRAINER_ITERATION;
	});
	thread->start();
	isTraining = true;
	return ASYNC_COMMAND;
}

CommandType NetworkTrainerProxy::Epoch()
{
	if (isTraining) {
		commandError = "Komenda jest w trakcie wykonywania";
		return INVALID_COMMAND;
	}
	if(!validateTrainingSet())
	{
		commandError = "Nieprawid³owe dane wejœciowe";
		return INVALID_COMMAND;
	}
	context.trainer.get()->setTrainingSet(context.inputDataTraining, context.idealDataTraining);

	this->thread->setFunction([&] (CommandThread* th){
		-context.trainer;
			const int currentEpoch = context.trainer.get()->epochsCount();
			context.previousEpochOutput.clear();
			do{
				context.trainer.get()->iteration();
				context.previousEpochOutput.push_back(matrix::deep_copy(context.network.getOutput()));
			} while (currentEpoch == context.trainer.get()->epochsCount());
		+context.trainer;
		isTraining = false;
		return TRAINER_EPOCH;
	});
	thread->start();
	isTraining = true;
	return ASYNC_COMMAND;
}

CommandType NetworkTrainerProxy::CheckThreshold(QString check, QString value)
{
	if (check == "false")
		checkErrorThreshold = false;
	if (check == "true") {
		checkErrorThreshold = true;
		bool ok;
		const double v = value.toDouble(&ok);
		if (ok && v != 0)
			errorThreshold = v;
	}
	return TRAINER_CHECKTHRESHOLD;
}

CommandType NetworkTrainerProxy::Train(QString epochsCount)
{
	if (isTraining) {
		commandError = "Komenda jest w trakcie wykonywania";
		return INVALID_COMMAND;
	}
	if (!validateTrainingSet())
	{
		commandError = "Nieprawid³owe dane wejœciowe";
		return INVALID_COMMAND;
	}
	if (checkErrorThreshold)
		if (context.trainer.get()->lastEpochTrainingError() <= errorThreshold)
		{
			commandError = QString("Blad sieci jest nie wiekszy niz %1").arg(errorThreshold);
			return INVALID_COMMAND;
		}
	bool ok = false;
	context.trainer.get()->setTrainingSet(context.inputDataTraining, context.idealDataTraining);
	const int epochs = epochsCount.toInt(&ok);
	if (ok && epochs > 0) {
		CommandThread* epochThread = new CommandThread([=] (CommandThread*){
				-context.trainer;
				const int currentEpoch = context.trainer.get()->epochsCount();
				context.previousEpochOutput.clear();
				do {
					context.trainer.get()->iteration();
					context.previousEpochOutput.push_back(matrix::deep_copy(context.network.getOutput()));
				} while (currentEpoch == context.trainer.get()->epochsCount());
				context.previousEpochInput = context.trainer.get()->getTrainingInput();
				context.trainer.get()->setEpochOutputInInitialOrder(context.previousEpochOutput);
				+context.trainer;
				return TRAINER_TRAIN_EPOCH;
		}, Q_NULLPTR);
		
		this->thread->setFunction([=](CommandThread* th) {
			for (int i = 0; i < epochs; i++)
			{
				epochThread->start();
				epochThread->wait();
				if(isTraining == false)
					return TRAINER_TRAIN_LAST_EPOCH;
				if (checkErrorThreshold)
					if (context.trainer.get()->lastEpochTrainingError() <= errorThreshold)
						break;
			}
			
			isTraining = false;
			return TRAINER_TRAIN_LAST_EPOCH;
		});


		bool c = connect(epochThread, &CommandThread::completed, this, &NetworkTrainerProxy::asyncCommandCompleted, Qt::BlockingQueuedConnection);
		Q_ASSERT(c);

		thread->start();
		isTraining = true;
		return TRAINER_TRAIN;
	}
}

CommandType NetworkTrainerProxy::SetRandomizeTrainingData(bool value)
{
	context.trainer.get()->setRandomizeTrainingData(value);
	return IGNORE_CMD;
}


