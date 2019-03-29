#include "ExampleNetworkDialog.h"
#include <QFile>
#include <QTextStream>

#include "NeuralNetworkProxy.h"
#include <qmath.h>

ExampleNetworkDialog::ExampleNetworkDialog(QWidget *parent)
	: QDialog(parent), painter(&dContext.networkContext.network)
{
	ui.setupUi(this);
	connect(ui.listWidget, &QListWidget::itemActivated, this, &ExampleNetworkDialog::onItemActivated);
	connect(ui.okBtn, &QPushButton::pressed, this, &ExampleNetworkDialog::onOkClicked);
	painter.setGraphicsView(ui.graphicsView);
	netProxy = new NeuralNetworkProxy(dContext.networkContext, this);
	dContext.commandEngine.addProxy(*netProxy);
	dContext.commandEngine.addHandler(painter);
	ui.listWidget->addItem("net-1-20-1-TanH.txt");
	ui.listWidget->addItem("net 1-20-20-1-TanH.txt");
	ui.listWidget->addItem("net 1-20-20-20-1-TanH.txt");
}

void ExampleNetworkDialog::onItemActivated(QListWidgetItem* item)
{
	netProxy->Clear();
	selectedPath = QString(":/net_examples/net_examples/%1").arg(item->text());
	QFile f(selectedPath);
	f.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream txt(&f);
	while (!txt.atEnd())
		dContext.commandEngine.executeCommand(txt.readLine());
	f.close();
	QMatrix matrix;
	qreal scale = qPow(qreal(2), (110 - 250) / qreal(50));
	matrix.scale(scale, scale);
	ui.graphicsView->setMatrix(matrix);
}

void ExampleNetworkDialog::onOkClicked()
{
	accept();
}
