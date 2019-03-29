#pragma once
#include "lib/qcustomplot.h"

class ScatterColorMapPlot :
	public QCustomPlot
{
public:
	ScatterColorMapPlot(QWidget *parent = 0);
	~ScatterColorMapPlot();
	void setTotalCells(int cellsX, int cellsY);
	void getTotalCells(double &x, double &y);
	void setXPlotRange(double lower, double upper);
	void getXPlotRange(double &x, double &y);
	void setYPlotRange(double lower, double upper);
	void getYPlotRange(double &x, double &y);

	void setClassCount(int count);
	QColor getClassColor(double classNum);
	void addColorMapData(double cordX, double cordY, double y);
	void addScatterData(double x, double y, double classNum);
	void addScatterData(QVector<double> x, QVector<double> y);
	void resetScatterPlot();
	void resetColorMap();



private:
	void setColorRange(int lower, int upper);
	int cellsX = 200;
	int cellsY = 200;
	int classCount = 3;
	double lowerYPlotRange = 0;
	double upperYPlotRange = 1;
	double lowerXPlotRange = 0;
	double upperXPlotRange = 1;
	QCPColorMap *colorMap;
	QCPColorScale *colorScale;
	QCPColorGradient grad;
	QVector<QCPScatterStyle> classToStyle;
	QCPScatterStyle myScatter;
};

