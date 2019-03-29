#include "LayerDialog.h"
#include "FFNetworkLayer.h"
#include "StatisticalFunctions.h"

LayerDialog::LayerDialog(QWidget *parent, Layer *layer)
	: QDialog(parent), weightsModel(layer->getBackingLayer()->getWeights(), this), biasesModel(layer->getBackingLayer()->getBiases(), this), layer(layer)
{
	weightsModel.setVerticalHeader("Neuron");
	weightsModel.setHorizontalHeader("Weight");
	biasesModel.setVerticalHeader("Neuron");
	biasesModel.setHorizontalHeader("Bias");
	ui.setupUi(this);
	ui.tableView->setModel(&weightsModel);
	ui.tableView_2->setModel(&biasesModel);
	ui.meanstdLabel->setText(QString::number(arithmectic_mean(standard_deviation(layer->getBackingLayer()->getWeights()))));
	ui.activationFunctionsCombo->addItems(activationFunctionNames);
	cmbStartIndex = activationFunctionNames.indexOf(QString::fromStdString(layer->getBackingLayer()->getActivationFunctionName()), 0);
	ui.activationFunctionsCombo->setCurrentIndex(
		cmbStartIndex
	);
	connect(ui.okBtn, &QPushButton::clicked, this ,&LayerDialog::onOkClicked);
	connect(ui.randomBtn, &QPushButton::pressed, [=] {
		layer->getBackingLayer()->randomizeWeights("01");
		ui.meanstdLabel->setText(QString::number(arithmectic_mean(standard_deviation(layer->getBackingLayer()->getWeights()))));
		weightsModel.updateData();
	});
	connect(ui.randomNBtn, &QPushButton::pressed, [=] {
		layer->getBackingLayer()->randomizeWeights("N01");
		ui.meanstdLabel->setText(QString::number(arithmectic_mean(standard_deviation(layer->getBackingLayer()->getWeights()))));
		weightsModel.updateData();
	});
}

void LayerDialog::onOkClicked(bool c)
{
	if(ui.activationFunctionsCombo->currentIndex() != cmbStartIndex)
	{
		emit dialogCmd(QString("ChangeActivationFunction(%1,%2)").arg(layer->getIndex()).arg(ui.activationFunctionsCombo->currentText()));
	}
	accept();
}
