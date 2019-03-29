#pragma once
#include <QObject>
#include "OutputPlot.h"
#include "NetworkErrorPlot.h"
#include "TrainerDialog.h"
#include "ClassificationOutputPlot.h"
#include "AppContext.h"


class MainWindow;
enum CommandType;


class Trainer : public QObject, public CommandHandler
{
	Q_OBJECT
public:
	Trainer(AppContext &context, QWidget *parent = Q_NULLPTR);
	~Trainer() = default;
	AppContext &context;
	QPointer<OutputPlot> outputPlot;
	QPointer<NetworkErrorPlot> networkErrorPlot;
	QPointer<TrainerDialog> dialogTrainer;
	QPointer<ClassificationOutputPlot> classificationPlot;
	void processCommand(CommandType cmd);
	void handleCommand(CommandType cmd) override;
private:
	void handleEpochRegression(CommandType cmd);
	void handleEpochClassification(CommandType cmd);
	double fill(matrix &x);
public slots:
	void showTrainingDialog();

	void showNetworkErrorPlot();

	void showNetworkOutputPlot();

	void showClassificationOutputPlot();

};

