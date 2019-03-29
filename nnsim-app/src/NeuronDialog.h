#pragma once

#include <QDialog>
#include "ui_NeuronDialog.h"

#include "MatrixTableModel.h"


class Neuron;

class NeuronDialog : public QDialog
{
	Q_OBJECT

public:
	NeuronDialog(Neuron* neuron, QWidget *parent = Q_NULLPTR);
	~NeuronDialog(){}
private:
	Ui::NeuronDialog ui;
	MatrixTableModel weightsModel;
};
