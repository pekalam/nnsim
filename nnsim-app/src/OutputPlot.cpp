#include "OutputPlot.h"
#include "matrix.h"
OutputPlot::OutputPlot(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.plot->xAxis->setLabel("");
	ui.plot->yAxis->setLabel("");
	ui.plot->xAxis->setRange(-1, 1);
	ui.plot->yAxis->setRange(-1, 1);
	ui.plot->linePlot()->setAntialiased(false);
	ui.plot->setOpenGl(true);
}


void OutputPlot::setScatteredData(std::vector<matrix>& x, std::vector<matrix>& y)
{
	if (x.size() > 0 && y.size() > 0 && x.size() != y.size())
		return;
	if (x[0].rows() != y[0].rows())
		return;
	for (int i = 0; i < x.size(); i++)
	{
		for (int j = 0; j < x.at(i).rows(); j++)
		{
			ui.plot->scatterPlot()->addData(MX_R1_AS_VEC(j, x.at(i)), MX_R1_AS_VEC(j, y.at(i)));
		}
	}
	ui.plot->rescaleAxes();
}


void OutputPlot::addLineData(std::vector<matrix>& x, std::vector<matrix>& y)
{
	bool updateExisting = false;
	if (ui.plot->linePlot()->data()->size() == maxLineDataSize)
		updateExisting = true;
	double px, py;
	for (int i = 0; i < x.size(); i++)
	{
		for (int j = 0; j < x.at(i).rows(); j++) {
			px =MX_R1_AS_VEC(j, x.at(i));
			py = MX_R1_AS_VEC(j, y.at(i));
			if (!updateExisting)
				ui.plot->linePlot()->addData(px, py);
			else
				(ui.plot->linePlot()->data()->begin()+(i+j))->value = py;
		}
	}
	
	ui.plot->rescaleAxes();
}
