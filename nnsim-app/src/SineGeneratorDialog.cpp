#include "SineGeneratorDialog.h"
#include <QPushButton>
#include <cmath>
#include "matrix.h"

SineGeneratorDialog::SineGeneratorDialog(std::vector<matrix>* input, std::vector<matrix>* ideal, QWidget *parent)
	: QDialog(parent), input(input), ideal(ideal), sineX(nullptr), sineY(nullptr), setSize(0)
{
	ui.setupUi(this);
	connect(ui.generateBtn, &QPushButton::clicked, this, &SineGeneratorDialog::onGenerateClicked);
	connect(ui.okBtn, &QPushButton::clicked, this, &SineGeneratorDialog::onOkClicked);
	ui.plot->xAxis->setLabel("x [rad]");
	ui.plot->yAxis->setLabel("y");
	ui.plot->xAxis->setRange(0, 1);
	ui.plot->yAxis->setRange(-1, 1);
	ui.plot->replot();
}

SineGeneratorDialog::~SineGeneratorDialog()
{
	if (!sineX)
		delete sineX;
	if (!sineY)
		delete sineY;
}

void SineGeneratorDialog::onOkClicked(bool b)
{
	if (!sineX)
		delete sineX;
	if (!sineY)
		delete sineY;
	accept();
	
}

void SineGeneratorDialog::onGenerateClicked(bool b)
{
	if (ui.length->value() <= 0 || setSize <= 0)
		return;
	if (!sineX)
		delete sineX;
	if (!sineY)
		delete sineY;
	input->clear();
	ideal->clear();
	int trainSize = ui.length->value();
	sineX = new QVector<double>(ui.length->value());
	sineY = new QVector<double>(ui.length->value());

	

	double deg = (double)ui.start->value();
	double step = (double)ui.step->value();
	for (int i = 0; i < trainSize; i++)
	{
		double rad = deg * (M_PI / 180);
		(*sineX)[i] = rad;
		(*sineY)[i] = sin(rad);
		deg += step;
	}
	for (int i = 0; i < trainSize; i += setSize)
	{
		matrix tra(setSize, 1);
		matrix ide(setSize, 1);
		for (int j = 0; j < setSize; j++)
		{
			MX(j, 0, tra) = (*sineX)[i + j];
			MX(j, 0, ide) = (*sineY)[i + j];
		}
		input->push_back(tra);
		ideal->push_back(ide);
	}


	ui.plot->scatterPlotReset();
	ui.plot->scatterPlot()->addData(*sineX, *sineY);
	ui.plot->rescaleAxes();
	ui.plot->replot();
}
