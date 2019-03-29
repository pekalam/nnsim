#include "NeuronDialog.h"
#include "Layer.h"
#include "FFNetworkLayer.h"

NeuronDialog::NeuronDialog(Neuron* neuron, QWidget* parent) : QDialog(parent), 
weightsModel(neuron->getLayer()->getBackingLayer()->getWeights(), this)
{
	for(int i = 0; i < weightsModel.getMatrix().rows(); i++)
		if (i != neuron->index)
			weightsModel.hideRow(i);
	weightsModel.setHorizontalHeader("Weight");
	ui.setupUi(this);
	ui.tableView->setModel(&weightsModel);
	const double bias = MX(neuron->index,0,neuron->getLayer()->getBackingLayer()->getBiases());
	const double output = MX(neuron->index, 0, neuron->getLayer()->getBackingLayer()->getOutput());
	ui.biasLabel->setText(QString::number(bias));
	ui.outputLabel->setText(QString::number(output));
}

