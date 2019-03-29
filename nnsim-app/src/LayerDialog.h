#pragma once

#include <QDialog>
#include "ui_LayerDialog.h"
#include "MatrixTableModel.h"

#include "Layer.h"
#include "ActivationFunction.h"

class LayerDialog : public QDialog
{
	Q_OBJECT

public:
	LayerDialog(QWidget *parent, Layer *layer);
private:
	Layer *layer;
	QStringList activationFunctionNames = {"Linear", "Sigmoid", "TanH", "ReLU"};
	int cmbStartIndex;
	Ui::LayerDialog ui;
	MatrixTableModel weightsModel;
	MatrixTableModel biasesModel;
private slots:
	void onOkClicked(bool c);
signals:
	void dialogCmd(QString);
};
