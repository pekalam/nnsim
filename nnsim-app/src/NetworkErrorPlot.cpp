#include "NetworkErrorPlot.h"
#include "AutoRescalingPlot.h"

NetworkErrorPlot::NetworkErrorPlot(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.plotResetBtn, &QPushButton::pressed, this, &NetworkErrorPlot::reset);
	initPlot();
}

void NetworkErrorPlot::initPlot()
{
	ui.plot->addGraph();
	ui.plot->xAxis->setLabel("Epoch");
	ui.plot->yAxis->setLabel("Network error");
	ui.plot->xAxis->setRange(1, 2);
	ui.plot->yAxis->setRange(0, 10);
	ui.plot->graph(0)->setAntialiased(false);
	ui.plot->setOpenGl(true);
}
