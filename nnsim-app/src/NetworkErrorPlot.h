#pragma once

#include <QDialog>
#include "ui_NetworkErrorPlot.h"

class NetworkErrorPlot : public QDialog
{
	Q_OBJECT

public:
	NetworkErrorPlot(QWidget *parent = Q_NULLPTR);
	void addData(double x, double y)
	{
		ui.label->setText(QString("Network error: %1").arg(y));
		ui.plot->addData(x, y);
		if(!prev)
		{
			prevX = x; prevY = y; prev = true;
		}
		else
		{
			double dydx = (y - prevY) / (x - prevX);
			ui.derivativeLabel->setText(QString::number(dydx,'g', 15));
			prevX = x;
			prevY = y;
		}
	}
	void update(QCustomPlot::RefreshPriority prior = QCustomPlot::RefreshPriority::rpImmediateRefresh){ ui.plot->replot(prior); }
public slots:
	void reset()
	{
		double lastX = ui.plot->xAxis->range().upper+1;
		ui.plot->graph(0)->setData(QVector<double>(), QVector<double>());
		ui.plot->xAxis->setRange(lastX, lastX+2);
		prev = false;
		update();
	}
	
private:
	double prevX, prevY;
	bool prev = false;
	void initPlot();
	Ui::NetworkErrorPlot ui;
};
