#include "Trainer.h"
#include "CommandType.h"
#include "NetworkContext.h"
#include "FFGradientDescend.h"
#include "WindowsContainer.h"
#include "MainWindow.h"
#include <QtConcurrent>


Trainer::Trainer(AppContext &context, QWidget *parent) : QObject(parent), CommandHandler(), context(context)
{
}

void Trainer::processCommand(CommandType cmd)
{
	switch(cmd)
	{
	case NETWORK_CLEAR:
	{
		context.networkContext.trainer.get()->reset();
		if (networkErrorPlot != nullptr) {
			networkErrorPlot->reset();
			networkErrorPlot->parentWidget()->close();
		}
		if (outputPlot != nullptr) {
			outputPlot->resetLineData();
			outputPlot->resetScatteredData();
			outputPlot->parentWidget()->close();
		}
		if (dialogTrainer != nullptr)
			dialogTrainer->parentWidget()->close();
		
		
		break;
	}
	case TRAINER_TRAIN:
	{
		if (outputPlot != nullptr) {
			outputPlot->resetLineData();
		}
			
		if (context.networkContext.getNetworkTaskType() == NetworkContext::NetworkTaskType::Regression) {
			showNetworkOutputPlot();
			outputPlot->setScatteredData(context.networkContext.inputDataTraining, context.networkContext.idealDataTraining);
			outputPlot->setMaxLineDataSize(context.networkContext.inputDataTraining.size());
			outputPlot->update();
		}
		else if (context.networkContext.getNetworkTaskType() == NetworkContext::NetworkTaskType::Classification)
		{
			handleEpochClassification(cmd);
		}
		showNetworkErrorPlot();
		dialogTrainer->parentWidget()->raise();
		break;
	}
	case TRAINER_ITERATION:
	{
		break;
	}
	case TRAINER_TRAIN_EPOCH:
	{
		showNetworkErrorPlot();
		networkErrorPlot->addData(context.networkContext.trainer.get()->epochsCount(), context.networkContext.trainer.get()->lastEpochTrainingError());
		networkErrorPlot->update();
		if(context.networkContext.getNetworkTaskType() == NetworkContext::NetworkTaskType::Regression)
			handleEpochRegression(cmd);
		if (context.networkContext.getNetworkTaskType() == NetworkContext::NetworkTaskType::Classification)
			handleEpochClassification(cmd);
		break;
	}
	case TRAINER_TRAIN_LAST_EPOCH:
	case TRAINER_EPOCH:
	{
		showNetworkErrorPlot();
		networkErrorPlot->update();
		if (context.networkContext.getNetworkTaskType() == NetworkContext::NetworkTaskType::Regression)
			handleEpochRegression(cmd);
		if (context.networkContext.getNetworkTaskType() == NetworkContext::NetworkTaskType::Classification) {
			handleEpochClassification(cmd);
		}
		break;
	}
	}
}

void Trainer::showTrainingDialog()
{
	if(dialogTrainer == nullptr)
	{
		dialogTrainer = new TrainerDialog(context);
		context.commandEngine.addHandler(*dialogTrainer);
		MainWindow::addChild(*dialogTrainer, 20, 25, 20, 0);
		
	}
	if (dialogTrainer->isVisible())
		return;
	dialogTrainer->show();
	
}

void Trainer::showNetworkErrorPlot()
{
	if(networkErrorPlot == nullptr)
	{
		networkErrorPlot = new NetworkErrorPlot(Q_NULLPTR);
		MainWindow::addChild(*networkErrorPlot, 60, 40, 0, 60);
	}
	if (networkErrorPlot->isVisible())
		return;
	networkErrorPlot->show();
}

void Trainer::showNetworkOutputPlot()
{
	if (outputPlot != nullptr && outputPlot->isVisible())
		return;
	if (classificationPlot != nullptr && classificationPlot->isVisible())
		classificationPlot->parentWidget()->close();
	
	if (outputPlot == nullptr) {
		outputPlot = new OutputPlot(Q_NULLPTR);
		MainWindow::addChild(*outputPlot, 40, 40, 60, 60);
	}
	
	outputPlot->show();
}

void Trainer::showClassificationOutputPlot()
{
	if (classificationPlot != nullptr && classificationPlot->isVisible())
		return;
	if (outputPlot != nullptr && outputPlot->isVisible())
		outputPlot->parentWidget()->close();
	if (classificationPlot == nullptr) {
		classificationPlot = new ClassificationOutputPlot(Q_NULLPTR);
		MainWindow::addChild(*classificationPlot, 100, 100);
		
		classificationPlot->setClassCount(3);
		classificationPlot->setResolution(100);
		classificationPlot->setDataRange(0, 9, 0, 9);
		
	}
	classificationPlot->show();
}

void Trainer::handleCommand(CommandType cmd)
{
	processCommand(cmd);
}

void Trainer::handleEpochRegression(CommandType cmd)
{
	if (cmd == TRAINER_TRAIN_EPOCH) {
		showNetworkOutputPlot();
		if (outputPlot->autoUpdate()) {
			outputPlot->addLineData(context.networkContext.previousEpochInput, context.networkContext.previousEpochOutput);
			outputPlot->updateLinePlot();
		}
	}
	else if(cmd == TRAINER_TRAIN_LAST_EPOCH || cmd == TRAINER_EPOCH)
	{
		showNetworkOutputPlot();
		outputPlot->addLineData(context.networkContext.previousEpochInput, context.networkContext.previousEpochOutput);
		outputPlot->updateLinePlot();
	}
}

void Trainer::handleEpochClassification(CommandType cmd)
{
	if (context.networkContext.network.getInputLayer().getNeuronsCount() > 2)
		return;
	if(cmd == TRAINER_TRAIN_LAST_EPOCH || cmd == TRAINER_EPOCH)
	{
		classificationPlot->fillMap(context.networkContext.network);
		classificationPlot->update();
	}
	if(cmd == TRAINER_TRAIN)
	{
		showClassificationOutputPlot();
		classificationPlot->resetScatteredData();
		std::vector<int> trainingSetClasses;
		for (int i = 0; i < context.networkContext.idealDataTraining.size(); i++)
		{
			for (int j = 0; j < context.networkContext.idealDataTraining.at(i).rows(); j++)
			{
				int cl = MX(j, 0, context.networkContext.idealDataTraining.at(i));
				if (cl == 1)
				{
					trainingSetClasses.push_back(j);
					break;
				}
			}
		}
		classificationPlot->setScatteredData(context.networkContext.inputDataTraining, trainingSetClasses);
		//classificationPlot->fillMap(context.networkContext.network);
	}
}

double Trainer::fill(matrix& x)
{
		const matrix &r = context.networkContext.network.evaluate(x);
		double m = MX(0, 0, r);
		int ind = 0;
		for (int i = 1; i < r.rows(); i++)
			if (MX(i, 0, r) > m)
			{
				m = MX(i, 0, r);
				ind = i;
			}
		return ind;
}
