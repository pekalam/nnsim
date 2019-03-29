#include "ClassificationOutputPlot.h"
#include "matrix.h"

ClassificationOutputPlot::ClassificationOutputPlot(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

ClassificationOutputPlot::~ClassificationOutputPlot()
{
}

void ClassificationOutputPlot::setScatteredData(std::vector<matrix>& trainingSetX, std::vector<int>& trainingSetClasses)
{
	for(int i = 0; i < trainingSetX.size(); i++)
	{
		double x = MX_R1_AS_VEC(0, trainingSetX[i]);
		double y = MX_R1_AS_VEC(1, trainingSetX[i]);
		int cl = trainingSetClasses.at(i);
		ui.plot->addScatterData(x,y,cl);
	}
}

void ClassificationOutputPlot::setDataRange(double xlower, double xupper, double ylower, double yupper)
{
	ui.plot->setXPlotRange(xlower, xupper);
	ui.plot->setYPlotRange(ylower, yupper);
	ui.plot->rescaleAxes();
}

void ClassificationOutputPlot::resetScatteredData()
{
	ui.plot->resetScatterPlot();
	ui.plot->replot();
}

void ClassificationOutputPlot::resetColorMapData()
{
	ui.plot->resetColorMap();
	ui.plot->replot();
}

void ClassificationOutputPlot::setResolution(int res)
{
	double lowX, uppX, lowY, uppY;
	ui.plot->getXPlotRange(lowX, uppX);
	ui.plot->getYPlotRange(lowY, uppY);
	double tot = (uppX - lowX) + (uppY - lowY);
	int rx = (uppX - lowX) * res / tot;
	int ry = (uppX - lowX) * res / tot;
	ui.plot->setTotalCells(rx, ry);
	ui.plot->rescaleAxes();
	ui.plot->replot();
}

void ClassificationOutputPlot::setClassCount(int count)
{
	ui.plot->setClassCount(count);
	ui.plot->rescaleAxes();
}

void ClassificationOutputPlot::fillMap(FFNeuralNetwork& net)
{
	double lowerXPlotRange, upperXPlotRange, upperYPlotRange, lowerYPlotRange, cellsX, cellsY;
	ui.plot->getXPlotRange(lowerXPlotRange, upperXPlotRange);
	ui.plot->getYPlotRange(lowerYPlotRange, upperYPlotRange);
	ui.plot->getTotalCells(cellsX, cellsY);

	double resX = (upperXPlotRange - lowerXPlotRange) / (double)cellsX;
	double resY = (upperYPlotRange - lowerYPlotRange) / (double)cellsY;
	matrix c(2, 1);
	for (double i = lowerXPlotRange; i <= upperXPlotRange; i += resX)
		for (double j = lowerYPlotRange; j <= upperYPlotRange; j += resY)
		{
			MX(0, 0, c) = i;
			MX(1, 0, c) = j;
			const matrix& r = net.evaluate(c);
			double m = MX(0, 0, r);
			int ind = 0;
			for (int k = 1; k < r.rows(); k++)
				if (MX(k, 0, r) > m)
				{
					m = MX(k, 0, r);
					ind = k;
				}
			ui.plot->addColorMapData(i, j, ind+1);
		}
	ui.plot->replot();
}


void ClassificationOutputPlot::update()
{
	ui.plot->replot();
}
