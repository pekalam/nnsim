#include "ScatterColorMapPlot.h"


ScatterColorMapPlot::ScatterColorMapPlot(QWidget *parent) : QCustomPlot(parent), myScatter()
{
	colorScale = new QCPColorScale(this);
	colorMap = new QCPColorMap(xAxis, yAxis);

	xAxis->setLabel("x");
	yAxis->setLabel("y");

	colorMap->data()->setSize(cellsX, cellsY);
	colorMap->data()->setRange(QCPRange(lowerXPlotRange, upperXPlotRange), QCPRange(lowerYPlotRange, upperYPlotRange));
	plotLayout()->addElement(0, 1, colorScale);
	colorScale->setType(QCPAxis::atRight);
	colorMap->setColorScale(colorScale);
	colorScale->setDataRange(QCPRange(0, classCount));
	colorScale->axis()->setLabel("Magnetic Field Strength");
	//colorMap->setGradient(QCPColorGradient::gpGeography);
	grad.setLevelCount(classCount);
	grad.setPeriodic(false);
	grad.setColorInterpolation(QCPColorGradient::ciRGB);
	grad.setColorStopAt(0, QColor::fromRgb(180, 180, 180));
	grad.setColorStopAt(1, QColor::fromRgb(0, 0, 0));
	colorMap->setGradient(grad);
	colorMap->setInterpolate(false);

	myScatter.setShape(QCPScatterStyle::ssCircle);
	myScatter.setPen(QPen(Qt::blue));
	myScatter.setBrush(Qt::green);
	myScatter.setSize(6);
	for (int i = 0; i < classCount; i++)
	{
		addGraph();
		//myScatter.setPen(QPen(getClassColor(i)));
		myScatter.setBrush(QBrush(getClassColor(i)));

		classToStyle.push_back(myScatter);
		graph(i)->setLineStyle(QCPGraph::LineStyle::lsNone);
		graph(i)->setScatterStyle(classToStyle.at(i));
	}
	colorMap->data()->clear(); colorMap->data()->clearAlpha();
	replot();
}

ScatterColorMapPlot::~ScatterColorMapPlot()
{

}

void ScatterColorMapPlot::setTotalCells(int cellsX, int cellsY)
{
	this->cellsX = cellsX;
	this->cellsY = cellsY;
	colorMap->data()->setSize(cellsX, cellsY);
}

void ScatterColorMapPlot::setXPlotRange(double lower, double upper)
{
	lowerXPlotRange = lower;
	upperXPlotRange = upper;
	colorMap->data()->setRange(QCPRange(lowerXPlotRange, upperXPlotRange), QCPRange(lowerYPlotRange, upperYPlotRange));
}

void ScatterColorMapPlot::setYPlotRange(double lower, double upper)
{
	lowerYPlotRange = lower;
	upperYPlotRange = upper;
	colorMap->data()->setRange(QCPRange(lowerXPlotRange, upperXPlotRange), QCPRange(lowerYPlotRange, upperYPlotRange));
}

void ScatterColorMapPlot::setClassCount(int count)
{
	if (count > classCount)
	{
		for (int i = classCount - 1; i < count; i++)
		{
			addGraph();
			//myScatter.setPen(QPen(getClassColor(i)));
			myScatter.setBrush(QBrush(getClassColor(i)));

			classToStyle.push_back(myScatter);
			graph(i)->setScatterStyle(classToStyle.at(i));
		}
	}
	else if (count < classCount)
	{
		for (int i = classCount - 1; i >= count; i--) {
			classToStyle.removeAt(i);
			removeGraph(i);
		}
	}
	classCount = count;
	grad.setLevelCount(classCount);
	colorMap->setGradient(grad);
	colorScale->setDataRange(QCPRange(0, classCount));
}

QColor ScatterColorMapPlot::getClassColor(double classNum)
{
	double pos = (classCount) / (double)classCount * (classNum + 1);
	QColor c = grad.color(pos, QCPRange(0, classCount));
	return c;
}

void ScatterColorMapPlot::addColorMapData(double cordX, double cordY, double y)
{
	int cx, cy;
	colorMap->data()->coordToCell(cordX, cordY, &cx, &cy);
	colorMap->data()->setCell(cx, cy, y);
}

void ScatterColorMapPlot::addScatterData(double x, double y, double classNum)
{
	graph(classNum)->addData(x, y);
	replot();
}

void ScatterColorMapPlot::addScatterData(QVector<double> x, QVector<double> y)
{
	graph(0)->addData(x, y);
}

void ScatterColorMapPlot::resetScatterPlot()
{
	for (int i = 0; i < classCount; i++)
		graph(i)->setData(QVector<double>(), QVector<double>());
}

void ScatterColorMapPlot::resetColorMap()
{
	colorMap->data()->clear();
}


void ScatterColorMapPlot::getTotalCells(double& x, double& y)
{
	x = cellsX;
	y = cellsY;
}

void ScatterColorMapPlot::getYPlotRange(double& x, double& y)
{
	x = lowerYPlotRange;
	y = upperYPlotRange;
}

void ScatterColorMapPlot::getXPlotRange(double& x, double& y)
{
	x = lowerXPlotRange;
	y = upperXPlotRange;
}
