#pragma once
#include "lib/qcustomplot.h"

class ScatterLinePlot :
	public QCustomPlot
{
public:
	ScatterLinePlot(QWidget *parent = 0);
	~ScatterLinePlot();
	QCPGraph* scatterPlot() { return graph(0); }
	QCPGraph* linePlot() { return graph(1); }
	void scatterPlotReset() { scatterPlot()->data()->clear();}
	void linePlotReset() { linePlot()->data()->clear();}
	void switchToLineLayer() { setCurrentLayer("lineL"); }
	void switchToScatterLayer() { setCurrentLayer("main");}
	void replotScatteredPlot() { scatterPlot()->rescaleAxes(); layer("axes")->replot(); layer("main")->replot(); }
	void replotLinePlot() { linePlot()->rescaleAxes(); layer("axes")->replot(); layer("lineL")->replot(); }
private:
	QCPScatterStyle myScatter;
};

