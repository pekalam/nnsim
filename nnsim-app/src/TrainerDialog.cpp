#include "TrainerDialog.h"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include "FFGradientDescend.h"

TrainerDialog::TrainerDialog(AppContext &context, QWidget *parent) : QDialog(parent), appContext(context), CommandHandler()
{
	ui.setupUi(this);
	bool ok;
	ok = connect(ui.trainBtn, &QPushButton::pressed, this, &TrainerDialog::onTrainClicked);
	Q_ASSERT(ok);
	ok = connect(ui.applyBtn, &QPushButton::pressed, this, &TrainerDialog::onApplyClicked);
	Q_ASSERT(ok);
	ok = connect(ui.momentum, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &TrainerDialog::onMomentumChange);
	Q_ASSERT(ok);
	ok = connect(ui.lerningRate, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &TrainerDialog::onLearningRateChanged);
	Q_ASSERT(ok);
	ok = connect(ui.stopBtn, &QPushButton::pressed, this, &TrainerDialog::onStopClicked);
	Q_ASSERT(ok);
	ok = connect(ui.randomize, &QCheckBox::stateChanged, this, &TrainerDialog::onRandomizeChecked);
	Q_ASSERT(ok);
	ok = connect(ui.thresholdCheckBox, &QCheckBox::stateChanged, this, &TrainerDialog::onThresholdChecked);
	Q_ASSERT(ok);
	ui.threshold->setDisabled(!checkThreshold);
	ui.momentum->setValue(appContext.networkContext.trainer.get()->momentum);
	ui.lerningRate->setValue(appContext.networkContext.trainer.get()->learningRate);
	ui.randomize->setChecked(false);
	randomize = false;
	lastLearningRt = ui.lerningRate->value();
	lastMomentum = ui.momentum->value();
	lastErrorThreshold = ui.threshold->value();
	ui.applyBtn->setDefault(false);
	ui.applyBtn->setAutoDefault(false);

	if(context.networkContext.getNetworkTaskType() == NetworkContext::Classification)
		ui.netType->setText("Classification network");
	else
		ui.netType->setText("Regression network");
}

void TrainerDialog::handleCommand(CommandType cmd)
{
	if (cmd == TRAINER_TRAIN) {
		trainingStarted();
	}
	if (cmd == TRAINER_TRAIN_LAST_EPOCH) {
		trainingCompleted();
	}
	if(cmd == TRAINER_TRAIN_EPOCH)
	{
		updateTrainer();
	}
};
void TrainerDialog::closeEvent(QCloseEvent* event)
{
	appContext.commandEngine.removeHandler(*this);
}

/*Tab 1 event Handlers*/

void TrainerDialog::onRandomizeChecked(int state)
{
	if (state == Qt::Checked)
		randomize = true;
	else if (state == Qt::Unchecked)
		randomize = false;
}
void TrainerDialog::onThresholdChecked(int state)
{
	if (state == Qt::Checked)
		checkThreshold = false;
	else if(state == Qt::Unchecked)
		checkThreshold = true;
	else
		return;
	ui.threshold->setDisabled(!checkThreshold);
	
}
void TrainerDialog::onMomentumChange(double v)
{
	if (v != lastMomentum)
		ui.applyBtn->setDefault(true);
	else if (v == lastMomentum && lastMomentum == ui.momentum->value())
		ui.applyBtn->setDefault(false);
}
void TrainerDialog::onLearningRateChanged(double v)
{
	if (v != lastLearningRt)
		ui.applyBtn->setDefault(true);
	else if (v == lastLearningRt && lastLearningRt == ui.lerningRate->value())
		ui.applyBtn->setDefault(false);
}
void TrainerDialog::onErrorThresholdChanged(double v)
{
	if(v != lastErrorThreshold)
		ui.applyBtn->setDefault(true);
	else if(v == lastErrorThreshold)
		ui.applyBtn->setDefault(false);
}
void TrainerDialog::onApplyClicked()
{
	needUpdate = true;
	if (!isTraining)
		updateTrainer();
	ui.applyBtn->setDefault(false);
}
void TrainerDialog::onTrainClicked()
{
	needUpdate = true;
	updateTrainer();
	appContext.commandEngine.executeCommand(QString("trainer.Train(%1)").arg(ui.epochs->value()));
}

/**/


/*Tab 2 event handlers*/

/*Event handlers*/

void TrainerDialog::updateTrainer()
{
	if (needUpdate)
	{
		const double momentum = ui.momentum->value();
		const double lrate = ui.lerningRate->value();
		if (appContext.networkContext.trainer.get()->learningRate != lrate)
			appContext.networkContext.trainer.get()->learningRate = lrate;
		if (appContext.networkContext.trainer.get()->momentum != momentum)
			appContext.networkContext.trainer.get()->momentum = momentum;
		appContext.networkContext.trainer.get()->setRandomizeTrainingData(randomize);
		if (lastCheckThreshold != checkThreshold || lastErrorThreshold != ui.threshold->value())
			appContext.commandEngine.executeCommand(QString("trainer.CheckThreshold(%1,%2)")
				.arg(checkThreshold ? "true" : "false").arg(QString::number(ui.threshold->value())));
		lastErrorThreshold = ui.threshold->value();
		lastCheckThreshold = checkThreshold;
		lastLearningRt = lrate;
		lastMomentum = momentum;
		needUpdate = false;
	}
}
void TrainerDialog::onStopClicked()
{
	appContext.commandEngine.stopAsyncCommands();
}
void TrainerDialog::trainingCompleted()
{
	ui.trainBtn->setDisabled(false);
	ui.epochs->setDisabled(false);
	isTraining = false;
}
void TrainerDialog::trainingStarted()
{
	ui.trainBtn->setDisabled(true);
	ui.epochs->setDisabled(true);
	isTraining = true;
}
