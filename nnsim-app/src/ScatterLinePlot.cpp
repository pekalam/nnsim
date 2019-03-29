#include "ScatterLinePlot.h"



ScatterLinePlot::ScatterLinePlot(QWidget *parent) : QCustomPlot(parent)
{
	addGraph(); //scattered 0
	addLayer("lineL");
	layer("lineL")->setMode(QCPLayer::LayerMode::lmBuffered);
	switchToLineLayer();
	addGraph(); //line 1


	myScatter.setShape(QCPScatterStyle::ssCircle);
	myScatter.setPen(QPen(Qt::blue));
	myScatter.setBrush(Qt::green);
	myScatter.setSize(6);
	graph(0)->setLineStyle(QCPGraph::LineStyle::lsNone);
	graph(0)->setScatterStyle(myScatter);
}


ScatterLinePlot::~ScatterLinePlot()
{
}
