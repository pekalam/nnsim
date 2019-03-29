#include "AutoRescalingPlot.h"

AutoRescalingPlot::AutoRescalingPlot(QWidget * parent)
	: QCustomPlot(parent)
{
}

void AutoRescalingPlot::addData(double x, double y)
{
	bool scale = false;
	graph(0)->addData(x, y);
	if (y <= yAxis->range().lower)
		scale = true;
	if (yAxis->range().upper <= y)
		scale = true;
	if (xAxis->range().upper <= x)
		scale = true;
	if (x <= xAxis->range().lower)
		scale = true;

	if (scale)
		rescaleAxes(true);
}
