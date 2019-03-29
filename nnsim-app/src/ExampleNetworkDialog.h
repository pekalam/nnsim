#pragma once
#include "ui/ui_ExampleNetworkDialog.h"
#include "NetworkPainter.h"
#include "AppContext.h"

class NeuralNetworkProxy;

class ExampleNetworkDialog : public QDialog
{
	Q_OBJECT

public:
	ExampleNetworkDialog(QWidget *parent = Q_NULLPTR);
	AppContext dContext;
	NetworkPainter painter;
	NeuralNetworkProxy* netProxy;
	QString selectedPath;
private:
	Ui::ExampleNetworkDialog ui;
private slots:
	void onItemActivated(QListWidgetItem *item);
	void onOkClicked();
};
