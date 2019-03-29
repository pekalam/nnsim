#pragma once

#include <QDialog>
#include "ui_TrainerDialog.h"
#include "ThreadSafe.h"
#include "AppContext.h"

class FFGradientDescend;

class TrainerDialog : public QDialog, public CommandHandler
{
	Q_OBJECT

public:
	TrainerDialog(AppContext &context, QWidget *parent = Q_NULLPTR);
	void handleCommand(CommandType cmd) override;
private:
	Ui::TrainerDialog ui;
	AppContext &appContext;
	double lastMomentum;
	double lastLearningRt;
	double lastErrorThreshold;
	bool lastCheckThreshold = false;
	bool randomize;
	bool checkThreshold = false;
	bool isTraining = false;
	bool needUpdate = false;
	void closeEvent(QCloseEvent* event) override;
private slots:
	void onRandomizeChecked(int state);
	void onThresholdChecked(int state);
	void onMomentumChange(double v);
	void onLearningRateChanged(double v);
	void onErrorThresholdChanged(double v);
	void onApplyClicked();
	void onTrainClicked();
public slots:
	void trainingCompleted();
	void trainingStarted();;
	void updateTrainer();
	void onStopClicked();
};
