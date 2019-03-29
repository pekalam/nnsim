#include "ClassificationDataGenerator.h"
#include "AppContext.h"
#include <QFile>
#include <iostream>


ClassificationDataGenerator::ClassificationDataGenerator(AppContext& context, QWidget *parent)
	: QDialog(parent), context(context)
{
	ui.setupUi(this);
	connect(ui.okBtn, &QPushButton::pressed, [&]
	{
		genData();
		accept();
	});
}

ClassificationDataGenerator::~ClassificationDataGenerator()
{
}

void ClassificationDataGenerator::genData()
{
	if(context.networkContext.network.getInputLayer().getNeuronsCount() != 2 || 
		context.networkContext.network.getOutputLayer().getNeuronsCount() != 3)
	{
		reject();
		return;
	}
	QString path = QString(":/example_data/classification_data/iris.txt");
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly)) {
		reject();
		return;
	}
	context.networkContext.inputDataTraining.clear();
	context.networkContext.idealDataTraining.clear();
	
	while (!file.atEnd()) {
		QString line = file.readLine();
		QStringList l = line.split(',');
		matrix x(2, 1);
		matrix y(3, 1, 0);
		MX(0, 0, x) = l[0].toDouble();
		MX(1, 0, x) = l[2].toDouble();
		int cl = l[4].toInt();
		MX(cl, 0, y) = 1;
		context.networkContext.inputDataTraining.push_back(x);
		context.networkContext.idealDataTraining.push_back(y);
	}
	for(int i = 0; i < context.networkContext.inputDataTraining.size(); i++)
	{
		QString c = QString("%1,%2,  %3,%4,%5")
		.arg(context.networkContext.inputDataTraining.at(i).getAt(0, 0))
			.arg((double)context.networkContext.inputDataTraining.at(i).getAt(1, 0))
			.arg((double)context.networkContext.idealDataTraining.at(i).getAt(0, 0))
			.arg((double)context.networkContext.idealDataTraining.at(i).getAt(1, 0))
			.arg((double)context.networkContext.idealDataTraining.at(i).getAt(2, 0));
		QByteArray ba = c.toLocal8Bit();
		qInfo(ba.data());

	}
	file.close();
	accept();
}
